//
// Created by paul on 22.09.18.
//

#ifndef POLYPROPYLENE_ENTITY_H
#define POLYPROPYLENE_ENTITY_H

#include <cassert>
#include <vector>
#include <optional>

#include "../definitions/CompilerDetection.h"
#include "../memory/AllocationService.h"
#include "../reflection/TypeMap.h"
#include "../event/EventService.h"

#include "PrototypeEntityPrefab.h"

// We have to create this workaround, because MSVC can't handle constexpr functions in enable_if.
#ifdef PAX_COMPILER_MSVC
#define PAX_GENERATE_EntityTemplateHeader(rettype, neg) \
template <class PropertyType, bool mult = PropertyType::IsMultiple()> \
typename std::enable_if<neg mult, rettype>::type
#else
#define PAX_GENERATE_EntityTemplateHeader(rettype, neg) \
template <class PropertyType> \
typename std::enable_if<neg PropertyType::IsMultiple(), rettype>::type
#endif

namespace PAX {
    /**
     * An Entity is a generic container for Properties.
     * @tparam Derived The class deriving from Entity.
     */
    template<class Derived>
    class Entity {
        static const std::vector<Property<Derived>*> EmptyPropertyVector;
        static AllocationService propertyAllocator;

        EventService _localEventService;
        bool active = false;

        TypeMap<Property<Derived>*> _singleProperties;
        TypeMap<std::vector<Property<Derived>*>> _multipleProperties;

        std::vector<Property<Derived>*> _allProperties; // Do we really want to have a copy of all pointers for easy access?

    public:
        Entity() = default;

        /**
         * Deletes all properties.
         */
        virtual ~Entity() {
            std::vector<Property<Derived>*> myProperties = getAllProperties();
            AllocationService & allocator = GetPropertyAllocator();

            while (!myProperties.empty()) {
                Property<Derived> * victim = myProperties.back();
                myProperties.pop_back();

                if (allocator.hasAllocated(victim)) {
                    TypeHandle victimType = victim->getClassType();
                    victim->~Property<Derived>();
                    GetPropertyAllocator().free(victimType.id, victim);
                }
            }
        }

    private:
        bool isValid(Property<Derived>* property) {
            if (property->owner)
                return false;

            return property->areDependenciesMetFor(*static_cast<Derived*>(this));
        }

        void registerproperty(Property<Derived>* property) {
            property->owner = static_cast<Derived*>(this);
            property->attached(*static_cast<Derived*>(this));
        }

        void unregisterproperty(Property<Derived>* property) {
            property->owner = nullptr;
            property->detached(*static_cast<Derived*>(this));
        }

    public:
        /**
         * Marks this Entity and all its properties as active.
         * This is a custom flag that can be used for enabling custom behaviour.
         * In video games, this entities can be marked active if they are part of the current scene or world.
         */
        void activate() {
            if (active) return;
            active = true;
            auto props = getAllProperties();
            for (Property<Derived> * p : props) {
                p->active = true;
                p->activated();
            }
        }

        /**
         * Marks this Entity and all its properties as inactive.
         * @ref activate()
         */
        void deactivate() {
            if (!active) return;
            active = false;
            auto props = getAllProperties();
            for (Property<Derived> * p : props) {
                p->active = false;
                p->deactivated();
            }
        }

        /**
         * Indicates if this entity together with all its properties is marked as active.
         * The semantics of this flag are user defined and thereby may be used for any purpose.
         * @ref activate()
         * @return True, iff this entity is marked active.
         */
        PAX_NODISCARD bool isActive() {
            return active;
        }

        /**
         * @return the AllocationService that is used for property (de-) allocation for properties for derived entitiy type Derived.
         */
        PAX_NODISCARD static AllocationService& GetPropertyAllocator() {
            return propertyAllocator;
        }

        /**
         * @return The internal EventService of this Entity that is used for internal communication between properties.
         */
        PAX_NODISCARD EventService& getEventService() {
            return _localEventService;
        }

        PAX_NODISCARD PrototypeEntityPrefab<Derived> toPrefab() const {
            return PrototypeEntityPrefab<Derived>(*this);
        }
        
        bool add(Property<Derived>* property) {
            if (isValid(property)) {
                property->PAX_INTERNAL(addTo)(*static_cast<Derived*>(this));
                registerproperty(property);
                _allProperties.push_back(property);
                if (active) {
                    property->active = true;
                    property->activated();
                }
                return true;
            }

            return false;
        }

        bool remove(Property<Derived>* property) {
            bool ret = property->PAX_INTERNAL(removeFrom)(*static_cast<Derived*>(this));
            unregisterproperty(property);
            Util::removeFromVector(_allProperties, property);
            if (active) {
                property->active = false;
                property->deactivated();
            }
            return ret;
        }

        PAX_GENERATE_EntityTemplateHeader(bool, !)
        has() const {
            return _singleProperties.count(typeid(PropertyType)) > 0;
        }

        PAX_GENERATE_EntityTemplateHeader(bool, )
        has() const {
            return _multipleProperties.count(typeid(PropertyType)) > 0;
        }

        template<class FirstPropertyType, class SecondPropertyType, class... FurtherPropertyTypees>
        PAX_NODISCARD bool has() const {
            bool X[] = { has<FirstPropertyType>(), has<SecondPropertyType>(), has<FurtherPropertyTypees>()... };

            constexpr int len = sizeof...(FurtherPropertyTypees) + 2;
            for (int i = 0; i < len; ++i)
                if (!X[i]) return false;

            return true;
        }

        PAX_NODISCARD bool has(const TypeId & type, std::optional<bool> isMultipleHint = {}) const {
            bool ret = false;

            if (isMultipleHint.value_or(true)) {
                ret = _multipleProperties.count(type) > 0;
                if (ret || isMultipleHint.has_value())
                    return ret;
            }

            assert(!ret);

            if (!isMultipleHint.value_or(false)) {
                ret = _singleProperties.count(type) > 0;
            }

            return ret;
        }

        PAX_GENERATE_EntityTemplateHeader(PropertyType*, !)
        get() {
            const auto& property = _singleProperties.find(typeid(PropertyType));
            if (property != _singleProperties.end())
                return static_cast<PropertyType*>(property->second);
            return nullptr;
        }

        PAX_GENERATE_EntityTemplateHeader(const std::vector<PropertyType*>&, )
        get() {
            const auto& properties = _multipleProperties.find(typeid(PropertyType));
            if (properties != _multipleProperties.end())
                return reinterpret_cast<std::vector<PropertyType*>&>(properties->second);
            else
                return *reinterpret_cast<const std::vector<PropertyType*>*>(&EmptyPropertyVector);
        }

        PAX_NODISCARD const std::vector<Property<Derived>*> & getAllProperties() const {
            return _allProperties;
        }

        /**
         * Returns a property from its runtime typehandle.
         * @param type The runtime type of the requested property.
         * @return The property of the given type which has single multiplicity (PAX_PROPERTY_IS_SINGLE), null if
         * this entity does not contain a property of that type or the type is not single.
         */
        PAX_NODISCARD Property<Derived> * getSingle(const TypeId & type) const {
            const auto & it = _singleProperties.find(type);

            if (it != _singleProperties.end()) {
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
        PAX_NODISCARD const std::vector<Property<Derived>*> & getMultiple(const TypeId & type) const {
            const auto & it = _multipleProperties.find(type);

            if (it != _multipleProperties.end()) {
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
        std::vector<Property<Derived>*> get(const TypeId & type) {
            // Copy is intended
            std::vector<Property<Derived>*> props = getMultiple(type);
            if (Property<Derived> * single = getSingle(type)) {
                props.emplace_back(single);
            }
            return props;
        }

        PAX_GENERATE_EntityTemplateHeader(PropertyType*, !)
        removeAll() {
            const auto& propertyIt = _singleProperties.contains(typeid(PropertyType));
            if (propertyIt != _singleProperties.end()) {
                PropertyType* property = static_cast<PropertyType*>(propertyIt->second);
                if (remove(property))
                    return property;
            }

            return nullptr;
        }

        PAX_GENERATE_EntityTemplateHeader(const std::vector<PropertyType*>&, )
        removeAll() {
            const auto& propertiesIt = _multipleProperties.contains(typeid(PropertyType));
            if (propertiesIt != _multipleProperties.end()) {
                // Copy to be able to return all removed instances
                auto properties = reinterpret_cast<std::vector<PropertyType*>>(_multipleProperties.get(typeid(PropertyType)));
                for (PropertyType* property : properties) {
                    if (!remove(property))
                        return EmptyPropertyVector;
                }

                return properties;
            }

            return EmptyPropertyVector;
        }

        
        /// DANGER ZONE: Functions for internal use only !!!!!!!!!!!!!!

        bool PAX_INTERNAL(addAsMultiple)(const TypeId & type, Property<Derived>* property) {
            _multipleProperties[type].push_back(property);
            return true;
        }

        bool PAX_INTERNAL(addAsSingle)(const TypeId & type, Property<Derived>* property) {
            if (_singleProperties.count(type)) {
                return false;
            } else {
                _singleProperties[type] = property;
            }

            return true;
        }

        bool PAX_INTERNAL(removeAsMultiple)(const TypeId & type, Property<Derived>* property) {
            std::vector<Property<Derived>*> &result = _multipleProperties.at(type);
            if (!Util::removeFromVector(result, property)) {
                return false;
            }

            // Remove vector if no propertys remain
            if (result.empty()) {
                _multipleProperties.erase(type);
            }

            return true;
        }

        bool PAX_INTERNAL(removeAsSingle)(const TypeId & type, Property<Derived>* property) {
            // The given property is not the property, that is registered for the given type.
            if (_singleProperties.at(type) != property) {
                return false;
            } else {
                return _singleProperties.erase(type) != 0;
            }
        }
    };

    template <class C>
    AllocationService Entity<C>::propertyAllocator;

    template <class C>
    const std::vector<Property<C>*> Entity<C>::EmptyPropertyVector(0);
}

#undef PAX_GENERATE_PropertyContainerFunctionTemplateHeader

#endif //POLYPROPYLENE_ENTITY_H
