//
// Created by paul on 22.09.18.
//

#ifndef POLYPROPYLENE_PROPERTY_H
#define POLYPROPYLENE_PROPERTY_H

#include "polypropylene/reflection/Reflectable.h"

#include "Entity.h"
#include "PropertyFactory.h"
#include "event/PropertyAttachedEvent.h"
#include "event/PropertyDetachedEvent.h"

namespace PAX {
    template<class TEntityType>
    class Property : public Reflectable {
        friend Entity<TEntityType, typename TEntityType::PropertyType>;

    public:
        using EntityType = TEntityType;
        static constexpr bool IsMultiple() { return true; }

    private:
        TEntityType * owner = nullptr;

    protected:
        virtual bool PAX_INTERNAL(addTo)(TEntityType & entity) PAX_NON_CONST { return true; }
        /*{
            if (entity.PAX_INTERNAL(addAsMultiple)(typeid(Property<TEntityType>), this)) {
                ::PAX::PropertyAttachedEvent<TEntityType, Property<TEntityType>> event(this, &entity);
                entity.getEventService()(event);
                return true;
            }

            return false;
        }//*/

        virtual bool PAX_INTERNAL(removeFrom)(TEntityType & entity) PAX_NON_CONST { return true; }
        /*{
            if (entity.PAX_INTERNAL(removeAsMultiple)(typeid(Property<TEntityType>), this)) {
                ::PAX::PropertyDetachedEvent<TEntityType, Property<TEntityType>> event(this, &entity);
                entity.getEventService()(event);
                return true;
            }

            return false;
        }//*/

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
         * Creates a copy of this property by considering all fields specified by the @ref getMetadata() method.
         * @return A clone of this property.
         */
        PAX_NODISCARD virtual typename TEntityType::PropertyType * clone() {
            // Theoretically, the return type does not have to be TEntityType::PropertyType.
            // However, it is the only return type making sense because no other types of properties can be added to EntityType.

            // TODO: It would be nice if we can make this method const.
            //       This is not trivial however because getMetadata()
            //       is not const because of all the pointers we retrieve from there.
            TEntityType::PropertyType * clone = PropertyFactoryRegister<TEntityType>::getFactoryFor(getClassType().id)->create();
            ClassMetadata cloneMetadata = clone->getMetadata();
            getMetadata().writeTo(cloneMetadata);
            clone->PAX_INTERNAL(created)();
            return clone;
        }

        /**
         * Callback that is invoked when the property was created by a prefab or cloned from another property.
         * Fields declared in Metadata (@ref getMetadata()) can be assumed to be initialised if values for them
         * were specified in the creating object (e.g., prefab).
         */
        virtual void PAX_INTERNAL(created)() {}
    };
}

#include "PropertyAnnotations.h"

#endif //POLYPROPYLENE_PROPERTY_H
