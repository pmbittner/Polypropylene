//
// Created by Paul on 30.12.2018.
//

#ifndef POLYPROPYLENE_PROPERTYREMOVEDEVENT_H
#define POLYPROPYLENE_PROPERTYREMOVEDEVENT_H

#include "EntityEvent.h"

namespace PAX {
    template <typename EntityType, class Prop>
    struct PropertyDetachedEvent : public EntityEvent<EntityType> {
        Prop * property;

        PropertyDetachedEvent(Prop* prop, EntityType* entity)
                : EntityEvent<EntityType>(entity), property(prop) {}
    };
}

#endif //POLYPROPYLENE_PROPERTYREMOVEDEVENT_H
