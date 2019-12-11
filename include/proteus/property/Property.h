//
// Created by paul on 22.09.18.
//

#ifndef PAXENGINE3_PROPERTY_H
#define PAXENGINE3_PROPERTY_H

#include "../reflection/TypeHandle.h"
#include "../macros/Definitions.h"
#include "construction/PropertyFactory.h"

namespace PAX {
    template<class C>
    class PropertyContainer;
    class ContentProvider;

    template<class C>
    class Property {
        friend class PropertyContainer<C>;
        friend class IPropertyFactory<C>;

    public:
        using Container = C;
        static constexpr bool IsMultiple() { return true; }

    private:
        bool active = false;
        C* owner = nullptr;

    protected:
        virtual bool PAX_INTERNAL(addTo)(Container & container) PAX_NON_CONST {
            if (container.PAX_INTERNAL(addAsMultiple)(paxtypeid(Property<C>), this)) {
                ::PAX::PropertyAttachedEvent<Container, Property<C>> event(this, &container);
                container.getEventService()(event);
                return true;
            }

            return false;
        }

        virtual bool PAX_INTERNAL(removeFrom)(Container & container) PAX_NON_CONST {
            if (container.PAX_INTERNAL(removeAsMultiple)(paxtypeid(Property<C>), this)) {
                ::PAX::PropertyDetachedEvent<Container, Property<C>> event(this, &container);
                container.getEventService()(event);
                return true;
            }

            return false;
        }

        virtual void activated() {}
        virtual void deactivated() {}

        virtual void attached(C &) {}
        virtual void detached(C &) {}

        virtual void initializeFromProvider(ContentProvider & provider) = 0;

    public:
        Property() : owner(nullptr) {}
        virtual ~Property() = default;

        C* getOwner() const { return owner; }

        PAX_NODISCARD virtual const TypeHandle& getClassType() const = 0;
        PAX_NODISCARD virtual bool isMultiple() const { return IsMultiple(); }
        virtual bool areDependenciesMetFor(const C&) const { return true; }

        bool isActive() { return active; }
    };

    /// Provide an implementation for this pure virtual function to allow calling it from subtypes.
    template<typename C>
    void Property<C>::initializeFromProvider(ContentProvider & provider) {}
}

#include "PropertyAnnotations.h"

#endif //PAXENGINE3_PROPERTY_H
