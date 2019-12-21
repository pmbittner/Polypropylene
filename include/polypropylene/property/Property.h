//
// Created by paul on 22.09.18.
//

#ifndef POLYPROPYLENE_PROPERTY_H
#define POLYPROPYLENE_PROPERTY_H

#include <polypropylene/reflection/ClassMetadata.h>
#include "../reflection/TypeHandle.h"
#include "../definitions/Definitions.h"
#include "PropertyFactory.h"

#include "event/PropertyAttachedEvent.h"
#include "event/PropertyDetachedEvent.h"

namespace PAX {
    template<class C>
    class Entity;
    class ContentProvider;

    template<class E>
    class Property {
        friend class Entity<E>;
        friend class IPropertyFactory<E>;

    public:
        using EntityType = E;
        static constexpr bool IsMultiple() { return true; }

    private:
        bool active = false;
        EntityType* owner = nullptr;

    protected:
        virtual bool PAX_INTERNAL(addTo)(E & entity) PAX_NON_CONST {
            if (entity.PAX_INTERNAL(addAsMultiple)(paxtypeid(Property<E>), this)) {
                ::PAX::PropertyAttachedEvent<E, Property<E>> event(this, &entity);
                entity.getEventService()(event);
                return true;
            }

            return false;
        }

        virtual bool PAX_INTERNAL(removeFrom)(E & entity) PAX_NON_CONST {
            if (entity.PAX_INTERNAL(removeAsMultiple)(paxtypeid(Property<E>), this)) {
                ::PAX::PropertyDetachedEvent<E, Property<E>> event(this, &entity);
                entity.getEventService()(event);
                return true;
            }

            return false;
        }

        virtual void activated() {}
        virtual void deactivated() {}

        virtual void attached(E &) {}
        virtual void detached(E &) {}

    public:
        Property() : owner(nullptr) {}
        virtual ~Property() = default;

        PAX_NODISCARD E * getOwner() const { return owner; }

        PAX_NODISCARD virtual const TypeHandle& getClassType() const = 0;
        PAX_NODISCARD virtual ClassMetadata getMetadata() { return {}; }
        PAX_NODISCARD virtual bool isMultiple() const { return IsMultiple(); }
        PAX_NODISCARD virtual bool areDependenciesMetFor(const E&) const { return true; }

        PAX_NODISCARD bool isActive() const { return active; }
    };
}

#include "PropertyAnnotations.h"

#endif //POLYPROPYLENE_PROPERTY_H
