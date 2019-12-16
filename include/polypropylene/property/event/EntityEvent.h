//
// Created by Paul on 30.12.2018.
//

#ifndef POLYPROPYLENE_ENTITYEVENT_H
#define POLYPROPYLENE_ENTITYEVENT_H

#include <polypropylene/event/Event.h>

namespace PAX {
    template<typename C>
    class Entity;

    template<typename EntityType>
    struct EntityEvent : public Event {
    public:
        EntityType * entity = nullptr;
        explicit EntityEvent(EntityType * entity) : entity(entity) {}
    };
}

#endif //POLYPROPYLENE_ENTITYEVENT_H
