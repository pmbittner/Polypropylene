//
// Created by Paul on 30.12.2018.
//

#ifndef POLYPROPYLENE_PROPERTYADDEDEVENT_H
#define POLYPROPYLENE_PROPERTYADDEDEVENT_H

#include "EntityEvent.h"

namespace PAX {
    template <typename EntityType, class Prop>
    struct PropertyAttachedEvent : public EntityEvent<EntityType> {
        Prop * property;

        PropertyAttachedEvent(Prop * prop, EntityType * entity)
                : EntityEvent<EntityType>(entity), property(prop) {}
    };
}

#endif //POLYPROPYLENE_PROPERTYADDEDEVENT_H
