//
// Created by paul on 06.01.19.
//

#ifndef POLYPROPYLENE_PROPERTYFACTORY_H
#define POLYPROPYLENE_PROPERTYFACTORY_H

#include <string>
#include <unordered_map>

#include "polypropylene/log/Errors.h"

#include "polypropylene/reflection/TypeMap.h"
#include "polypropylene/serialisation/ClassMetadataSerialiser.h"

#define PAX_PROPERTY_REGISTER_AS(PropertyType, Name) \
do { \
    PAX_CONSTEXPR_IF (!PropertyType::IsAbstract()) { \
        ::PAX::PropertyFactoryRegister<PropertyType::EntityType>::registerFactory<PropertyType>(Name); \
    } \
} while(0)

#define PAX_PROPERTY_REGISTER(PropertyType) PAX_PROPERTY_REGISTER_AS(PropertyType, #PropertyType)

namespace PAX {
    template<typename C>
    class Property;

    template<class C>
    class IPropertyFactory {
        std::string name;

    protected:
        explicit IPropertyFactory(const std::string & name) noexcept : name(name) {}
        virtual ~IPropertyFactory() = default;

    public:
        /**
         * Creates a property allocated with the allocator of Entity<C>.
         * @return A newly heap-allocated Property.
         */
        PAX_NODISCARD virtual Property<C> * create() const = 0;
        PAX_NODISCARD virtual TypeHandle getPropertyType() const = 0;
        PAX_NODISCARD virtual bool isPropertyMultiple() const = 0;
        PAX_NODISCARD const std::string & getPropertyName() const {
            return name;
        }
    };

    template<class C>
    class PropertyFactoryRegister {
        using MapType = std::unordered_map<std::string, IPropertyFactory<C> *>;

        // Use this method to save the map to avoid the Static Initialization Order Fiasko.
        static std::unordered_map<std::string, IPropertyFactory<C> *> & getNameMap() noexcept {
            static std::unordered_map<std::string, IPropertyFactory<C> *> map;
            return map;
        }

        // Use this method to save the map to avoid the Static Initialization Order Fiasko.
        static UnorderedTypeMap<IPropertyFactory<C> *> & getTypeMap() noexcept {
            static UnorderedTypeMap<IPropertyFactory<C> *> map;
            return map;
        }

    protected:
        PropertyFactoryRegister() noexcept = default;

    public:
        virtual ~PropertyFactoryRegister() = default;

        static IPropertyFactory<C> * getFactoryFor(const std::string & name) {
            const auto &map = getNameMap();
            const auto &it = map.find(name);

            if (it != map.end()) {
                return it->second;
            } else {
                PAX_THROW_RUNTIME_ERROR("No factory is registered for the name \"" << name << "\"!");
            }
        }

        static IPropertyFactory<C> * getFactoryFor(const TypeId & type) {
            const auto &map = getTypeMap();
            const auto &it = map.find(type);

            if (it != map.end()) {
                return it->second;
            } else {
                PAX_THROW_RUNTIME_ERROR("No factory is registered for the type \"" << type.name() << "\"!");
            }
        }

        template<typename PropertyType>
        static void registerFactory(const std::string & name);
    };

    template<typename PropertyType, typename C>
    class PropertyFactory : public IPropertyFactory<C> {
    public:
        explicit PropertyFactory(const std::string & name) noexcept : IPropertyFactory<C>(name) {}
        virtual ~PropertyFactory() = default;

        PAX_NODISCARD PropertyType * create() const override {
            return new (C::GetPropertyAllocator().template allocate<PropertyType>()) PropertyType();
        }

        PAX_NODISCARD TypeHandle getPropertyType() const override {
            return paxtypeof(PropertyType);
        }

        PAX_NODISCARD bool isPropertyMultiple() const override {
            return PropertyType::IsMultiple();
        }
    };

    template<class C>
    template<typename PropertyType>
    void PropertyFactoryRegister<C>::registerFactory(const std::string & name) {
        static PropertyFactory<PropertyType, C> factory(name);
        PropertyFactoryRegister<C>::getNameMap()[name] = &factory;
        PropertyFactoryRegister<C>::getTypeMap()[typeid(PropertyType)] = &factory;
    }
}

#endif