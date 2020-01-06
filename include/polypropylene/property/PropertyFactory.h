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

#define PAX_PROPERTY_REGISTER_AS(TPropertyType, Name) \
do { \
    PAX_CONSTEXPR_IF (!TPropertyType::IsAbstract()) { \
        ::PAX::PropertyFactoryRegister<TPropertyType::EntityType>::registerFactory<TPropertyType>(Name); \
    } \
} while(0)

#define PAX_PROPERTY_REGISTER(TPropertyType) PAX_PROPERTY_REGISTER_AS(TPropertyType, #TPropertyType)

namespace PAX {
    template<class TEntityType>
    class IPropertyFactory {
        std::string name;

    protected:
        explicit IPropertyFactory(const std::string & name) noexcept : name(name) {}
        virtual ~IPropertyFactory() = default;

    public:
        /**
         * Creates a property allocated with the allocator of Entity<TEntityType>.
         * @return A newly heap-allocated Property.
         */
        PAX_NODISCARD virtual typename TEntityType::PropertyType * create() const = 0;
        PAX_NODISCARD virtual TypeHandle getPropertyType() const = 0;
        PAX_NODISCARD virtual bool isPropertyMultiple() const = 0;

        PAX_NODISCARD const std::string & getPropertyName() const {
            return name;
        }
    };

    template<class TEntityType>
    class PropertyFactoryRegister {
        using MapType = std::unordered_map<std::string, IPropertyFactory<TEntityType> *>;

        // Use this method to save the map to avoid the Static Initialization Order Fiasko.
        static std::unordered_map<std::string, IPropertyFactory<TEntityType> *> & getNameMap() noexcept {
            static std::unordered_map<std::string, IPropertyFactory<TEntityType> *> map;
            return map;
        }

        // Use this method to save the map to avoid the Static Initialization Order Fiasko.
        static UnorderedTypeMap<IPropertyFactory<TEntityType> *> & getTypeMap() noexcept {
            static UnorderedTypeMap<IPropertyFactory<TEntityType> *> map;
            return map;
        }

    protected:
        PropertyFactoryRegister() noexcept = default;

    public:
        virtual ~PropertyFactoryRegister() = default;

        static IPropertyFactory<TEntityType> * getFactoryFor(const std::string & name) {
            const auto &map = getNameMap();
            const auto &it = map.find(name);

            if (it != map.end()) {
                return it->second;
            } else {
                PAX_THROW_RUNTIME_ERROR("No factory is registered for the name \"" << name << "\"!");
            }
        }

        static IPropertyFactory<TEntityType> * getFactoryFor(const TypeId & type) {
            const auto &map = getTypeMap();
            const auto &it = map.find(type);

            if (it != map.end()) {
                return it->second;
            } else {
                PAX_THROW_RUNTIME_ERROR("No factory is registered for the type \"" << type.name() << "\"!");
            }
        }

        /**
         * Creates a copy of the given property by considering all fields specified by the @ref getMetadata() method.
         * @param propertyToClone The property that should be cloned.
         * @return A clone of this property.
         */
        PAX_NODISCARD static typename TEntityType::PropertyType * clone(typename TEntityType::PropertyType * propertyToClone) {
            // Theoretically, the return type does not have to be TEntityType::PropertyType.
            // However, it is the only return type making sense because no other types of properties can be added to TEntityType.
            typename TEntityType::PropertyType * clone = PropertyFactoryRegister<TEntityType>::getFactoryFor(propertyToClone->getClassType().id)->create();
            ClassMetadata cloneMetadata = clone->getMetadata();
            propertyToClone->getMetadata().writeTo(cloneMetadata);
            clone->PAX_INTERNAL(created)();
            return clone;
        }

        template<typename TPropertyType>
        static void registerFactory(const std::string & name);
    };

    template<typename TPropertyType, typename TEntityType>
    class PropertyFactory : public IPropertyFactory<TEntityType> {
    public:
        explicit PropertyFactory(const std::string & name) noexcept : IPropertyFactory<TEntityType>(name) {}
        virtual ~PropertyFactory() = default;

        PAX_NODISCARD TPropertyType * create() const override {
            return new (TEntityType::GetPropertyAllocator().template allocate<TPropertyType>()) TPropertyType();
        }

        PAX_NODISCARD TypeHandle getPropertyType() const override {
            return paxtypeof(TPropertyType);
        }

        PAX_NODISCARD bool isPropertyMultiple() const override {
            return TPropertyType::IsMultiple();
        }
    };

    template<class TEntityType>
    template<typename TPropertyType>
    void PropertyFactoryRegister<TEntityType>::registerFactory(const std::string & name) {
        static PropertyFactory<TPropertyType, TEntityType> factory(name);
        PropertyFactoryRegister<TEntityType>::getNameMap()[name] = &factory;
        PropertyFactoryRegister<TEntityType>::getTypeMap()[typeid(TPropertyType)] = &factory;
    }
}

#endif