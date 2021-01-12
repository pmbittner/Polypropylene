//
// Created by paul on 22.09.18.
//

#ifndef POLYPROPYLENE_ENTITY_H
#define POLYPROPYLENE_ENTITY_H

#include <cassert>
#include <vector>
#include <optional>

#include "ForwardDeclarations.h"
#include "Property.h"
#include "../definitions/CompilerDetection.h"
#include "../memory/AllocationService.h"
#include "../reflection/TypeMap.h"
#include "../event/EventService.h"

// We have to create this workaround, because MSVC can't handle constexpr functions in enable_if.
#ifdef PAX_COMPILER_MSVC
#define PAX_GENERATE_EntityTemplateHeader(rettype, neg) \
template <class TProperty, bool mult = TProperty::IsMultiple()> \
typename std::enable_if<neg mult, rettype>::type PAX_MAYBEUNUSED
#else
#define PAX_GENERATE_EntityTemplateHeader(rettype, neg) \
template <class TProperty> \
typename std::enable_if<neg TProperty::IsMultiple(), rettype>::type
#endif

namespace PAX {
    /**
     * An Entity is a generic container for Properties.
     * @tparam TDerived The class deriving from Entity.
     * @tparam TRootProperty The type of properties that can be added to this container.
     * This constraints possible properties added to this Entity to derivatives of TRootProperty or TRootProperty itself.
     */
    template<class TDerived, class TRootProperty>
    class Entity {
        static_assert(
                std::is_convertible<TRootProperty*, Property<TDerived>*>::value,
                "Given TRootProperty is not defined or does not inherit from Property<TDerived>!");

        static const std::vector<TRootProperty*> & GetEmptyPropertyVector() {
            static std::vector<TRootProperty*> emptyvec(0);
            return emptyvec;
        }

        EventService localEventService;

        TypeMap<TRootProperty*> singleProperties;
        TypeMap<std::vector<TRootProperty*>> multipleProperties;

    public:
        using EntityType = TDerived;
        using PropertyType = TRootProperty;

        Entity() = default;

        /**
         * Deletes all properties that were allocated with the AllocationService
         * of this Entity (e.g., those that were allocated with pax_new).
         */
        virtual ~Entity() {
            AllocationService & allocator = GetAllocationService();
            const std::vector<TRootProperty*> & props = getAllProperties();
            for (TRootProperty * propToDelete : props) {
                if (allocator.hasAllocated(propToDelete)) {
                    const TypeHandle pType = propToDelete->getClassType();
                    propToDelete->~TRootProperty();
                    GetAllocationService().free(pType.id, propToDelete);
                }
            }
        }

    private:
        bool isValid(Property<TDerived> * property) {
            if (property->owner) {
                return false;
            }

            return property->areDependenciesMetFor(*static_cast<TDerived*>(this));
        }

        inline void registerProperty(TRootProperty* property) {
            property->owner = static_cast<TDerived*>(this);
            property->attached(*static_cast<TDerived*>(this));
            onPropertyAdded(property);
        }

        inline void unregisterProperty(TRootProperty* property) {
            property->owner = nullptr;
            property->detached(*static_cast<TDerived*>(this));
            onPropertyRemoved(property);
        }

    protected:
        virtual void onPropertyAdded(TRootProperty * property) {};
        virtual void onPropertyRemoved(TRootProperty * property) {};

    public:
        /**
         * @return The AllocationService that is used for entity and property (de-) allocation for derived entitiy type TDerived.
         */
        PAX_NODISCARD static AllocationService& GetAllocationService() {
            static AllocationService allocator;
            return allocator;
        }

        /**
         * @return The internal EventService of this Entity that is used for internal communication between properties.
         */
        PAX_NODISCARD EventService& getEventService() {
            return localEventService;
        }

        PAX_NODISCARD PrototypeEntityPrefab<TDerived> toPrefab() const;
        
        bool add(TRootProperty* property) {
            if (isValid(property)) {
                // We need static_cast to access the protected method addTo.
                // The cast is valid because TRootProperty is a subclass of Property<TDerived>.
                // We cant access 'addTo' on TRootProperty because it is protected there.
                // We can however access it on Property<TDerived> because we are a friend of it.
                static_cast<Property<TDerived>*>(property)->PAX_INTERNAL(addTo)(*static_cast<TDerived*>(this));
                registerProperty(property);
                return true;
            }

            return false;
        }

        bool remove(TRootProperty* property) {
            // static_cast is necessary for the same reason described in method @ref add.
            if (static_cast<Property<TDerived>*>(property)->PAX_INTERNAL(removeFrom)(*static_cast<TDerived*>(this))) {
                unregisterProperty(property);
                return true;
            }

            return false;
        }

        PAX_GENERATE_EntityTemplateHeader(bool, !)
        has() const {
            return singleProperties.count(typeid(TProperty)) > 0;
        }

        PAX_GENERATE_EntityTemplateHeader(bool, )
        has() const {
            return multipleProperties.count(typeid(TProperty)) > 0;
        }

        template<class FirstTPropertyType, class SecondTPropertyType, class... FurtherTPropertyTypees>
        PAX_NODISCARD bool has() const {
            bool X[] = { has<FirstTPropertyType>(), has<SecondTPropertyType>(), has<FurtherTPropertyTypees>()... };

            constexpr int len = sizeof...(FurtherTPropertyTypees) + 2;
            for (int i = 0; i < len; ++i)
                if (!X[i]) return false;

            return true;
        }

        PAX_NODISCARD bool has(const TypeId & type, std::optional<bool> isMultipleHint = {}) const {
            bool ret = false;

            if (isMultipleHint.value_or(true)) {
                ret = multipleProperties.count(type) > 0;
                if (ret || isMultipleHint.has_value())
                    return ret;
            }

            assert(!ret);

            if (!isMultipleHint.value_or(false)) {
                ret = singleProperties.count(type) > 0;
            }

            return ret;
        }

        PAX_GENERATE_EntityTemplateHeader(TProperty*, !)
        get() const {
            const auto& property = singleProperties.find(typeid(TProperty));
            if (property != singleProperties.end())
                return static_cast<TProperty*>(property->second);
            return nullptr;
        }

        PAX_GENERATE_EntityTemplateHeader(const std::vector<TProperty*>&, )
        get() const {
            const auto& properties = multipleProperties.find(typeid(TProperty));
            if (properties != multipleProperties.end())
                return reinterpret_cast<const std::vector<TProperty*>&>(properties->second);
            else
                return *reinterpret_cast<const std::vector<TProperty*>*>(&GetEmptyPropertyVector());
        }

        PAX_NODISCARD const std::vector<TRootProperty*> & getAllProperties() const {
            return get<TRootProperty>();
        }

        /**
         * Returns a property from its runtime typehandle.
         * @param type The runtime type of the requested property.
         * @return The property of the given type which has single multiplicity (PAX_PROPERTY_IS_SINGLE), null if
         * this entity does not contain a property of that type or the type is not single.
         */
        PAX_NODISCARD TRootProperty * getSingle(const TypeId & type) const {
            const auto & it = singleProperties.find(type);

            if (it != singleProperties.end()) {
                return it->second;
            }

            return nullptr;
        }

        /**
         * Returns a property from its runtime typehandle.
         * @param type The runtime type of the requested property.
         * @return The properties of the given type which has multiple multiplicity (PAX_PROPERTY_IS_MULTIPLE), an
         * empty vector if this entity does not contain a property of that type or the type is not multiple.
         */
        PAX_NODISCARD const std::vector<TRootProperty*> & getMultiple(const TypeId & type) const {
            const auto & it = multipleProperties.find(type);

            if (it != multipleProperties.end()) {
                return it->second;
            }

            return GetEmptyPropertyVector();
        }

        /**
         * Returns all properties of the given type contained in this entity.
         * @param type The runtime type of the requested properties.
         * @return The properties of the given type with any multiplicity.
         * The returned vector is empty, iff this entity does not contain any properties of the given type.
         * If the property type is single (PAX_PROPERTY_IS_SINGLE), the returned vector will have size 1.
         * If the property type is multiple (PAX_PROPERTY_IS_MULTIPLE), the returned vector will contain all properties
         * of the given type.
         */
        std::vector<TRootProperty*> get(const TypeId & type) {
            // Copy is intended
            std::vector<TRootProperty*> props = getMultiple(type);
            if (TRootProperty * single = getSingle(type)) {
                props.emplace_back(single);
            }
            return props;
        }

        PAX_GENERATE_EntityTemplateHeader(TProperty*, !)
        removeAll() {
            const auto& propertyIt = singleProperties.contains(typeid(TProperty));
            if (propertyIt != singleProperties.end()) {
                TProperty* property = static_cast<TProperty*>(propertyIt->second);
                if (remove(property))
                    return property;
            }

            return nullptr;
        }

        PAX_GENERATE_EntityTemplateHeader(const std::vector<TProperty*>&, )
        removeAll() {
            const auto& propertiesIt = multipleProperties.contains(typeid(TProperty));
            if (propertiesIt != multipleProperties.end()) {
                // Copy to be able to return all removed instances
                auto properties = reinterpret_cast<std::vector<TProperty*>>(multipleProperties.get(typeid(TProperty)));
                for (TProperty* property : properties) {
                    if (!remove(property))
                        return GetEmptyPropertyVector();
                }

                return properties;
            }

            return GetEmptyPropertyVector();
        }

        
        /// DANGER ZONE: Functions for internal use only !!!!!!!!!!!!!!

        bool PAX_INTERNAL(addAsMultiple)(const TypeId & type, TRootProperty* property) {
            multipleProperties[type].push_back(property);
            return true;
        }

        bool PAX_INTERNAL(addAsSingle)(const TypeId & type, TRootProperty* property) {
            if (singleProperties.count(type)) {
                return false;
            } else {
                singleProperties[type] = property;
            }

            return true;
        }

        bool PAX_INTERNAL(removeAsMultiple)(const TypeId & type, TRootProperty* property) {
            std::vector<TRootProperty*> &result = multipleProperties.at(type);
            if (!Util::removeFromVector(result, property)) {
                return false;
            }

            // Remove vector if no propertys remain
            if (result.empty()) {
                multipleProperties.erase(type);
            }

            return true;
        }

        bool PAX_INTERNAL(removeAsSingle)(const TypeId & type, TRootProperty* property) {
            // The given property is not the property, that is registered for the given type.
            if (singleProperties.at(type) != property) {
                return false;
            } else {
                return singleProperties.erase(type) != 0;
            }
        }
    };
}

#undef PAX_GENERATE_PropertyContainerFunctionTemplateHeader

#include "PrototypeEntityPrefab.h"

#endif //POLYPROPYLENE_ENTITY_H