//
// Created by paul on 22.09.18.
//

#ifndef POLYPROPYLENE_ENTITY_H
#define POLYPROPYLENE_ENTITY_H

#include <vector>
#include <iostream>

#include "../macros/CompilerDetection.h"
#include "../memory/AllocationService.h"
#include "../reflection/TypeMap.h"
#include "../event/EventService.h"

#include "Property.h"

// We have to create this workaround, because MSVC can't handle constexpr functions in enable_if.
#ifdef PAX_COMPILER_MSVC
#define PAX_GENERATE_PropertyContainerFunctionTemplateHeader(rettype, neg) \
template <class PropertyType, bool mult = PropertyType::IsMultiple()> \
typename std::enable_if<neg mult, rettype>::type
#else
#define PAX_GENERATE_PropertyContainerFunctionTemplateHeader(rettype, neg) \
template <class PropertyType> \
typename std::enable_if<neg PropertyType::IsMultiple(), rettype>::type
#endif

namespace PAX {
    template<class T>
    class Entity {
        static const std::vector<Property<T>*> EmptyPropertyVector;
        static AllocationService propertyAllocator;

        EventService _localEventService;
        bool active = false;

        TypeMap<Property<T>*> _singleProperties;
        TypeMap<std::vector<Property<T>*>> _multipleProperties;

        std::vector<Property<T>*> _allProperties; // Do we really want to have a copy of all pointers for easy access?

    public:
        Entity() = default;

        virtual ~Entity() {
            // 1.) Unattach everything to invoke detach events and not delete something twice.
            // 2.) Delete propertys as they should not be reused.

            while (!_singleProperties.empty()) {
                Property<T> * propToRemove = _singleProperties.begin()->second;
                remove(propToRemove);
                delete propToRemove;
            }

            while (!_multipleProperties.empty()) {
                const auto & it = _multipleProperties.begin();
                if (!it->second.empty()) {
                    Property<T> * propToRemove = it->second.front();
                    if (remove(propToRemove)) {
                        delete propToRemove;
                    } else {
                        std::cerr << "[PropertyContainer::~PropertyContainer] Invalid state: Removing property of type" << propToRemove->getClassType().name() << " unsuccessful!";
                    }
                }
            }

            _multipleProperties.clear();
        }

    private:
        bool isValid(Property<T>* property) {
            if (property->owner)
                return false;

            return property->areDependenciesMetFor(*static_cast<T*>(this));
        }

        void registerproperty(Property<T>* property) {
            property->owner = static_cast<T*>(this);
            property->attached(*static_cast<T*>(this));
        }

        void unregisterproperty(Property<T>* property) {
            property->owner = nullptr;
            property->detached(*static_cast<T*>(this));
        }

    protected:
        void activate() {
            if (active) return;
            active = true;
            auto props = getAll();
            for (Property<T> * p : props) {
                p->active = true;
                p->activated();
            }
        }

        void deactivate() {
            if (!active) return;
            active = false;
            auto props = getAll();
            for (Property<T> * p : props) {
                p->active = false;
                p->deactivated();
            }
        }

    public:
        static AllocationService& GetPropertyAllocator() {
            return propertyAllocator;
        }

        EventService& getEventService() {
            return _localEventService;
        }

        bool add(Property<T>* property) {
            if (isValid(property)) {
                property->PAX_INTERNAL(addTo)(*static_cast<T*>(this));
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

        bool remove(Property<T>* property) {
            bool ret = property->PAX_INTERNAL(removeFrom)(*static_cast<T*>(this));
            unregisterproperty(property);
            Util::removeFromVector(_allProperties, property);
            if (active) {
                property->active = false;
                property->deactivated();
            }
            return ret;
        }

        PAX_GENERATE_PropertyContainerFunctionTemplateHeader(bool, !)
        has() const {
            return _singleProperties.count(paxtypeid(PropertyType)) > 0;
        }

        PAX_GENERATE_PropertyContainerFunctionTemplateHeader(bool, )
        has() const {
            return _multipleProperties.count(paxtypeid(PropertyType)) > 0;
        }

        template<class FirstPropertyType, class SecondPropertyType, class... FurtherPropertyTypees>
        PAX_NODISCARD bool has() const {
            bool X[] = { has<FirstPropertyType>(), has<SecondPropertyType>(), has<FurtherPropertyTypees>()... };

            constexpr int len = sizeof...(FurtherPropertyTypees) + 2;
            for (int i = 0; i < len; ++i)
                if (!X[i]) return false;

            return true;
        }

        PAX_NODISCARD bool has(const TypeHandle & type, std::optional<bool> isMultipleHint = {}) const {
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

        PAX_GENERATE_PropertyContainerFunctionTemplateHeader(PropertyType*, !)
        get() {
            const auto& property = _singleProperties.find(paxtypeid(PropertyType));
            if (property != _singleProperties.end())
                return static_cast<PropertyType*>(property->second);
            return nullptr;
        }

        PAX_GENERATE_PropertyContainerFunctionTemplateHeader(const std::vector<PropertyType*>&, )
        get() {
            const auto& properties = _multipleProperties.find(paxtypeid(PropertyType));
            if (properties != _multipleProperties.end())
                return reinterpret_cast<std::vector<PropertyType*>&>(properties->second);
            else
                return *reinterpret_cast<const std::vector<PropertyType*>*>(&EmptyPropertyVector);
        }

        const std::vector<Property<T>*> & getAll() {
            return _allProperties;
        }

        Property<T> * getSingle(const TypeHandle & type) const {
            const auto & it = _singleProperties.find(type);

            if (it != _singleProperties.end()) {
                return it->second;
            }

            return nullptr;
        }

        const std::vector<Property<T>*> & getMultiple(const TypeHandle & type) const {
            const auto & it = _multipleProperties.find(type);

            if (it != _multipleProperties.end()) {
                return it->second;
            }

            return EmptyPropertyVector;
        }

        std::vector<Property<T>*> get(const TypeHandle & type) {
            // Copy is intended
            std::vector<Property<T>*> props = getMultiple(type);
            if (Property<T> * single = getSingle(type)) {
                props.emplace_back(single);
            }
            return props;
        }

        PAX_GENERATE_PropertyContainerFunctionTemplateHeader(PropertyType*, !)
        removeAll() {
            const auto& propertyIt = _singleProperties.contains(paxtypeid(PropertyType));
            if (propertyIt != _singleProperties.end()) {
                PropertyType* property = static_cast<PropertyType*>(propertyIt->second);
                if (remove(property))
                    return property;
            }

            return nullptr;
        }

        PAX_GENERATE_PropertyContainerFunctionTemplateHeader(const std::vector<PropertyType*>&, )
        removeAll() {
            const auto& propertiesIt = _multipleProperties.contains(paxtypeid(PropertyType));
            if (propertiesIt != _multipleProperties.end()) {
                // Copy to be able to return all removed instances
                auto properties = reinterpret_cast<std::vector<PropertyType*>>(_multipleProperties.get(paxtypeid(PropertyType)));
                for (PropertyType* property : properties) {
                    if (!remove(property))
                        return EmptyPropertyVector;
                }

                return properties;
            }

            return EmptyPropertyVector;
        }

        bool isActive() {
            return active;
        }

        bool PAX_INTERNAL(addAsMultiple)(const std::type_info & type, Property<T>* property) {
            _multipleProperties[type].push_back(property);
            return true;
        }

        bool PAX_INTERNAL(addAsSingle)(const std::type_info & type, Property<T>* property) {
            if (_singleProperties.count(type)) {
                return false;
            } else
                _singleProperties[type] = property;

            return true;
        }

        bool PAX_INTERNAL(removeAsMultiple)(const std::type_info & type, Property<T>* property) {
            std::vector<Property<T>*> &result = _multipleProperties.at(type);
            if (!Util::removeFromVector(result, property))
                return false;

            // Remove vector if no propertys remain
            if (result.empty())
                _multipleProperties.erase(type);

            return true;
        }

        bool PAX_INTERNAL(removeAsSingle)(const std::type_info & type, Property<T>* property) {
            // The given property is not the property, that is registered for the given type.
            if (_singleProperties.at(type) != property)
                return false;
            else
                return _singleProperties.erase(type) != 0;
        }
    };

    template <class C>
    AllocationService Entity<C>::propertyAllocator;

    template <class C>
    const std::vector<Property<C>*> Entity<C>::EmptyPropertyVector(0);
}

#undef PAX_GENERATE_PropertyContainerFunctionTemplateHeader

#endif //POLYPROPYLENE_ENTITY_H
