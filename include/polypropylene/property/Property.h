//
// Created by paul on 22.09.18.
//

#ifndef POLYPROPYLENE_PROPERTY_H
#define POLYPROPYLENE_PROPERTY_H

#include "polypropylene/reflection/Reflectable.h"

#include "PropertyFactory.h"
#include "event/PropertyAttachedEvent.h"
#include "event/PropertyDetachedEvent.h"

namespace PAX {
    template<class C>
    class Entity;

    template<class E>
    class Property : public Reflectable {
        friend class Entity<E>;

    public:
        using EntityType = E;
        static constexpr bool IsMultiple() { return true; }

    protected:
        virtual bool PAX_INTERNAL(addTo)(E & entity) PAX_NON_CONST {
            if (entity.PAX_INTERNAL(addAsMultiple)(typeid(Property<E>), this)) {
                ::PAX::PropertyAttachedEvent<E, Property<E>> event(this, &entity);
                entity.getEventService()(event);
                return true;
            }

            return false;
        }

        virtual bool PAX_INTERNAL(removeFrom)(E & entity) PAX_NON_CONST {
            if (entity.PAX_INTERNAL(removeAsMultiple)(typeid(Property<E>), this)) {
                ::PAX::PropertyDetachedEvent<E, Property<E>> event(this, &entity);
                entity.getEventService()(event);
                return true;
            }

            return false;
        }

        /**
         * Callback that is invoked when this property gets attached to an entity.
         * @param E The entity this property got attached to.
         */
        virtual void attached(E & entity) {}


        /**
         * Callback that is invoked when this property gets removed from an entity.
         * @param E The entity this property got removed from.
         */
        virtual void detached(E & entity) {}

    public:
        Property() = default;
        virtual ~Property() = default;

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
        PAX_NODISCARD virtual bool areDependenciesMetFor(const E & entity) const { return true; }

        /**
         * Creates a copy of this property by considering all fields specified by the @ref getMetadata() method.
         * @return A clone of this property.
         */
        PAX_NODISCARD virtual Property<E> * clone() {
            // TODO: It would be nice if we can make this method const.
            //       This is not trivial however because getMetadata()
            //       is not const because of all the pointers we retrieve from there.
            Property<E> * clone = PropertyFactoryRegister<E>::getFactoryFor(getClassType().id)->create();
            ClassMetadata cloneMetadata = clone->getMetadata();
            getMetadata().writeTo(cloneMetadata);
            clone->created();
            return clone;
        }
        
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
