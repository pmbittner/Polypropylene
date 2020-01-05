//
// Created by paul on 22.09.18.
//

#ifndef POLYPROPYLENE_ENTITY_H
#define POLYPROPYLENE_ENTITY_H

#include <cassert>
#include <vector>
#include <optional>

#include "ForwardDeclarations.h"
#include "../definitions/CompilerDetection.h"
#include "../memory/AllocationService.h"
#include "../reflection/TypeMap.h"
#include "../event/EventService.h"

// We have to create this workaround, because MSVC can't handle constexpr functions in enable_if.
#ifdef PAX_COMPILER_MSVC
#define PAX_GENERATE_EntityTemplateHeader(rettype, neg) \
template <class TParamPropertyType, bool mult = TParamPropertyType::IsMultiple()> \
typename std::enable_if<neg mult, rettype>::type
#else
#define PAX_GENERATE_EntityTemplateHeader(rettype, neg) \
template <class TParamPropertyType> \
typename std::enable_if<neg TParamPropertyType::IsMultiple(), rettype>::type
#endif

namespace PAX {
    /**
     * An Entity is a generic container for Properties.
     * @tparam TDerived The class deriving from Entity.
     * @tparam TPropertyType The type of properties that can be added to this container (defaults to Property<TDerived>).
     * This constraints possible properties added to this Entity to derivatives of TPropertyType or TPropertyType itself.
     */
    template<class TDerived, class TPropertyType>
    class Entity {
        static const std::vector<TPropertyType*> EmptyPropertyVector;
        static AllocationService propertyAllocator;

        EventService localEventService;

        TypeMap<TPropertyType*> singleProperties;
        TypeMap<std::vector<TPropertyType*>> multipleProperties;

        std::vector<TPropertyType*> allProperties; // Do we really want to have a copy of all pointers for easy access?

    public:
        using PropertyType = TPropertyType;

        Entity() = default;

        /**
         * Deletes all properties.
         */
        virtual ~Entity() {
            std::vector<TPropertyType*> myProperties = getProperties();
            AllocationService & allocator = GetPropertyAllocator();

            while (!myProperties.empty()) {
                TPropertyType * victim = myProperties.back();
                myProperties.pop_back();

                if (allocator.hasAllocated(victim)) {
                    TypeHandle victimType = victim->getClassType();
                    victim->~TPropertyType();
                    GetPropertyAllocator().free(victimType.id, victim);
                }
            }
        }

    private:
        bool isValid(TPropertyType* property) {
            if (property->owner) {
                return false;
            }

            return property->areDependenciesMetFor(*static_cast<TDerived*>(this));
        }

        inline void registerProperty(TPropertyType* property) {
            property->owner = static_cast<TDerived*>(this);
            property->attached(*static_cast<TDerived*>(this));
            allProperties.push_back(property);
        }

        inline void unregisterProperty(TPropertyType* property) {
            property->owner = nullptr;
            property->detached(*static_cast<TDerived*>(this));
            Util::removeFromVector(allProperties, property);
        }

    public:
        /**
         * @return the AllocationService that is used for property (de-) allocation for properties for derived entitiy type TDerived.
         */
        PAX_NODISCARD static AllocationService& GetPropertyAllocator() {
            return propertyAllocator;
        }

        /**
         * @return The internal EventService of this Entity that is used for internal communication between properties.
         */
        PAX_NODISCARD EventService& getEventService() {
            return localEventService;
        }

        PAX_NODISCARD PrototypeEntityPrefab<TDerived> toPrefab() const;
        
        bool add(TPropertyType* property) {
            if (isValid(property)) {
                // We need static_cast to access the protected method addTo.
                // The cast is valid because TPropertyType is a subclass of Property<TDerived>.
                // We cant access 'addTo' on TPropertyType because it is protected there.
                // We can however access it on Property<TDerived> because we are a friend of it.
                static_cast<Property<TDerived>*>(property)->PAX_INTERNAL(addTo)(*static_cast<TDerived*>(this));
                registerProperty(property);
                return true;
            }

            return false;
        }

        bool remove(TPropertyType* property) {
            // static_cast is necessary for the same reason described in method @ref add.
            if (static_cast<Property<TDerived>*>(property)->PAX_INTERNAL(removeFrom)(*static_cast<TDerived*>(this))) {
                unregisterProperty(property);
                return true;
            }

            return false;
        }

        PAX_GENERATE_EntityTemplateHeader(bool, !)
        has() const {
            return singleProperties.count(typeid(TParamPropertyType)) > 0;
        }

        PAX_GENERATE_EntityTemplateHeader(bool, )
        has() const {
            return multipleProperties.count(typeid(TParamPropertyType)) > 0;
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

        PAX_GENERATE_EntityTemplateHeader(TParamPropertyType*, !)
        get() {
            const auto& property = singleProperties.find(typeid(TParamPropertyType));
            if (property != singleProperties.end())
                return static_cast<TParamPropertyType*>(property->second);
            return nullptr;
        }

        PAX_GENERATE_EntityTemplateHeader(const std::vector<TParamPropertyType*>&, )
        get() {
            const auto& properties = multipleProperties.find(typeid(TParamPropertyType));
            if (properties != multipleProperties.end())
                return reinterpret_cast<std::vector<TParamPropertyType*>&>(properties->second);
            else
                return *reinterpret_cast<const std::vector<TParamPropertyType*>*>(&EmptyPropertyVector);
        }

        PAX_NODISCARD const std::vector<TPropertyType*> & getProperties() const {
            return allProperties;
        }

        /**
         * Returns a property from its runtime typehandle.
         * @param type The runtime type of the requested property.
         * @return The property of the given type which has single multiplicity (PAX_PROPERTY_IS_SINGLE), null if
         * this entity does not contain a property of that type or the type is not single.
         */
        PAX_NODISCARD TPropertyType * getSingle(const TypeId & type) const {
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
        PAX_NODISCARD const std::vector<TPropertyType*> & getMultiple(const TypeId & type) const {
            const auto & it = multipleProperties.find(type);

            if (it != multipleProperties.end()) {
                return it->second;
            }

            return EmptyPropertyVector;
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
        std::vector<TPropertyType*> get(const TypeId & type) {
            // Copy is intended
            std::vector<TPropertyType*> props = getMultiple(type);
            if (TPropertyType * single = getSingle(type)) {
                props.emplace_back(single);
            }
            return props;
        }

        PAX_GENERATE_EntityTemplateHeader(TParamPropertyType*, !)
        removeAll() {
            const auto& propertyIt = singleProperties.contains(typeid(TParamPropertyType));
            if (propertyIt != singleProperties.end()) {
                TParamPropertyType* property = static_cast<TParamPropertyType*>(propertyIt->second);
                if (remove(property))
                    return property;
            }

            return nullptr;
        }

        PAX_GENERATE_EntityTemplateHeader(const std::vector<TParamPropertyType*>&, )
        removeAll() {
            const auto& propertiesIt = multipleProperties.contains(typeid(TParamPropertyType));
            if (propertiesIt != multipleProperties.end()) {
                // Copy to be able to return all removed instances
                auto properties = reinterpret_cast<std::vector<TParamPropertyType*>>(multipleProperties.get(typeid(TParamPropertyType)));
                for (TParamPropertyType* property : properties) {
                    if (!remove(property))
                        return EmptyPropertyVector;
                }

                return properties;
            }

            return EmptyPropertyVector;
        }

        
        /// DANGER ZONE: Functions for internal use only !!!!!!!!!!!!!!

        bool PAX_INTERNAL(addAsMultiple)(const TypeId & type, TPropertyType* property) {
            multipleProperties[type].push_back(property);
            return true;
        }

        bool PAX_INTERNAL(addAsSingle)(const TypeId & type, TPropertyType* property) {
            if (singleProperties.count(type)) {
                return false;
            } else {
                singleProperties[type] = property;
            }

            return true;
        }

        bool PAX_INTERNAL(removeAsMultiple)(const TypeId & type, TPropertyType* property) {
            std::vector<TPropertyType*> &result = multipleProperties.at(type);
            if (!Util::removeFromVector(result, property)) {
                return false;
            }

            // Remove vector if no propertys remain
            if (result.empty()) {
                multipleProperties.erase(type);
            }

            return true;
        }

        bool PAX_INTERNAL(removeAsSingle)(const TypeId & type, TPropertyType* property) {
            // The given property is not the property, that is registered for the given type.
            if (singleProperties.at(type) != property) {
                return false;
            } else {
                return singleProperties.erase(type) != 0;
            }
        }
    };

    template <class D, class P>
    AllocationService Entity<D, P>::propertyAllocator;

    template <class D, class P>
    const std::vector<P*> Entity<D, P>::EmptyPropertyVector(0);
}

#undef PAX_GENERATE_PropertyContainerFunctionTemplateHeader

#include "PrototypeEntityPrefab.h"

#endif //POLYPROPYLENE_ENTITY_H