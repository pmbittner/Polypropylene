//
// Created by Paul on 30.12.2018.
//

#ifndef POLYPROPYLENE_PROPERTYADDEDEVENT_H
#define POLYPROPYLENE_PROPERTYADDEDEVENT_H

#include "PropertyContainerEvent.h"

namespace PAX {
    template <typename C, class Prop>
    struct PropertyAttachedEvent : public PropertyContainerEvent<C> {
        Prop * property;

        PropertyAttachedEvent(Prop* prop, PropertyContainer<C>* container)
                : PropertyContainerEvent<C>(container), property(prop) {}
    };
}

#endif //POLYPROPYLENE_PROPERTYADDEDEVENT_H
