//
// Created by paul on 22.09.18.
//

#ifndef POLYPROPYLENE_PROPERTY_H
#define POLYPROPYLENE_PROPERTY_H

#include "ForwardDeclarations.h"
#include "polypropylene/reflection/Reflectable.h"
#include "event/PropertyAttachedEvent.h"
#include "event/PropertyDetachedEvent.h"

/**
 * Convenience macro for creating properties and entities with the allocation service.
 * Use it as follows:
 *    Cheese * c = pax_new(Cheese)(arguments, for, constructor, here);
 * @param propOrEntityType The type of the property or entity you want to create.
 */
#define pax_new(propOrEntityType) \
new (propOrEntityType::EntityType::GetAllocationService().allocate(paxtypeof(propOrEntityType))) propOrEntityType

template<typename T>
void pax_delete(T * t) {
    T::EntityType::GetAllocationService().template deleteAndFree<T>(t);
}

namespace PAX {
    template<class TEntityType>
    class Property : public Reflectable {
        template<class, class>
        friend class Entity;

    public:
        using EntityType = TEntityType;
        static constexpr bool IsMultiple() { return true; }

    private:
        TEntityType * owner = nullptr;

    protected:
        virtual bool PAX_INTERNAL(addTo)(TEntityType & entity) PAX_NON_CONST { return true; }
        virtual bool PAX_INTERNAL(removeFrom)(TEntityType & entity) PAX_NON_CONST { return true; }

        /**
         * Callback that is invoked when this property gets attached to an entity.
         * @param TEntityType The entity this property got attached to.
         */
        virtual void attached(TEntityType & entity) {}

        /**
         * Callback that is invoked when this property gets removed from an entity.
         * @param TEntityType The entity this property got removed from.
         */
        virtual void detached(TEntityType & entity) {}

    public:
        Property() = default;
        virtual ~Property() = default;

        /**
         * @return The entity this property is attached to. Returns nullptr if this property is not attached to an entity.
         */
        PAX_NODISCARD TEntityType * getOwner() const { return owner; }

        /**
         * @return A TypeHandle identifying the actual type of the object (i.e., the derived class).
         */
        PAX_NODISCARD virtual const TypeHandle& getClassType() const = 0;

        /**
         * @return Reflection information on the fields of this property that should be considered for cloning,
         * (de-)serialisation, and prefabs.
         */
        PAX_NODISCARD ClassMetadata getMetadata() override { return {}; }

        /**
         * @return True if multiple instances of this property can be attached to the same entity.
         * Returns false if only a single instance is allowed per entity.
         */
        PAX_NODISCARD virtual bool isMultiple() const { return IsMultiple(); }

        /**
         * This method is invoked to resolve dependencies upon attaching this property to an entity.
         * Properties may depend on other properties, i.e., require their presence.
         * @param entity The entity this property should be attached to.
         * @return True if all dependencies if this property are met for the given entity.
         */
        PAX_NODISCARD virtual bool areDependenciesMetFor(const TEntityType & entity) const { return true; }

        /**
         * Callback that is invoked when the property was created by a prefab or cloned from another property.
         * Fields declared in Metadata (@ref getMetadata()) can be assumed to be initialised if values for them
         * were specified in the creating object (e.g., prefab).
         */
        virtual void created() {}
    };
}

#include "PropertyAnnotations.h"

#endif //POLYPROPYLENE_PROPERTY_H
