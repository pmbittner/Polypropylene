//
// Created by paul on 06.01.19.
//

#ifndef POLYPROPYLENE_PROPERTYFACTORY_H
#define POLYPROPYLENE_PROPERTYFACTORY_H

#include <string>
#include <unordered_map>

#include "polypropylene/serialisation/ContentProvider.h"

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
    public:
        IPropertyFactory() noexcept = default;
        virtual ~IPropertyFactory() {}

        PAX_NODISCARD virtual Property<C> * create(ContentProvider & contentProvider) const = 0;
        PAX_NODISCARD virtual TypeHandle getPropertyType() const = 0;
        PAX_NODISCARD virtual bool isPropertyMultiple() const = 0;
    };

    template<class C>
    class PropertyFactoryRegister {
        using MapType = std::unordered_map<std::string, IPropertyFactory<C> *>;

        // Use this method to save the map to avoid the Static Initialization Order Fiasko.
        static MapType & getMap() noexcept {
            static MapType map;
            return map;
        }

    protected:
        PropertyFactoryRegister() noexcept = default;

        static void registerFactory(const std::string &name, IPropertyFactory<C> *constructor) noexcept {
            getMap()[name] = constructor;
        }

    public:
        virtual ~PropertyFactoryRegister() = default;

        static IPropertyFactory<C> * getFactoryFor(const std::string &name) {
            const auto &map = getMap();
            const auto &it = map.find(name);

            if (it != map.end()) {
                return it->second;
            } else {
                PAX_THROW_RUNTIME_ERROR("No factory is registered for the name \"" << name << "\"!");
            }
        }

        template<typename PropertyType>
        static void registerFactory(const std::string & name);
    };

    template<typename PropertyType, typename C>
    class PropertyFactory : public IPropertyFactory<C> {
    public:
        explicit PropertyFactory(const std::string &name) noexcept : IPropertyFactory<C>() {}
        virtual ~PropertyFactory() = default;

        PAX_NODISCARD PropertyType * create(ContentProvider &contentProvider) const override {
            return new
#ifndef PAX_OVERWRITE_NEW_AND_DELETE_FOR_PROPERTIES
                (Entity<C>::GetPropertyAllocator().allocate<PropertyType>())
#endif
                PropertyType();
        }

        PAX_NODISCARD TypeHandle getPropertyType() const override {
            return paxtypeid(PropertyType);
        }

        PAX_NODISCARD bool isPropertyMultiple() const override {
            return PropertyType::IsMultiple();
        }
    };

    template<class C>
    template<typename PropertyType>
    void PropertyFactoryRegister<C>::registerFactory(const std::string & name) {
        static PropertyFactory<PropertyType, C> factory(name);
        PropertyFactoryRegister<C>::registerFactory(name, &factory);
    }
}

#endif