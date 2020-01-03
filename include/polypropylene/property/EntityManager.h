//
// Created by Paul on 23.03.2019.
//

#ifndef POLYPROPYLENE_ENTITYMANAGER_H
#define POLYPROPYLENE_ENTITYMANAGER_H

#include <set>

#include "Entity.h"

namespace PAX {
    template<typename EntityType>
    struct EntityAddedEvent : public EntityEvent<EntityType> {
        explicit EntityAddedEvent(EntityType * entity) : EntityEvent<EntityType>(entity) {}
    };

    template<typename EntityType>
    struct EntityRemovedEvent : public EntityEvent<EntityType> {
        explicit EntityRemovedEvent(EntityType * entity) : EntityEvent<EntityType>(entity) {}
    };

    /**
     * An EntityManager is a collection of entities.
     * It links the event services of all contained entities to allow communication between them.
     * The contained entities can either be accessed explicitly with getEntities() or implicitly by
     * iterating over the manager itself.
     * Using managers allows the usage of custom views on entities (@ref EntityManagerView).
     *
     * @tparam EntityType The concrete Entity type (i.e., the derived class)
     */
    template<typename EntityType>
    class EntityManager {
        std::set<EntityType*> entities;
        EventService & eventService;

    public:
        using iterator = typename decltype(entities)::iterator;
        using const_iterator = typename decltype(entities)::const_iterator;

        explicit EntityManager(EventService & eventService) : eventService(eventService) {

        }

        PAX_NODISCARD const std::set<EntityType*> & getEntities() const {
            return entities;
        }

        bool add(EntityType * entity) {
            if (entities.insert(entity).second) {
                entity->getEventService().setParent(&eventService);
                EntityAddedEvent<EntityType> e(entity);
                eventService(e);
                return true;
            }

            return false;
        }

        bool remove(EntityType * entity) {
            if (entities.erase(entity)) {
                entity->getEventService().setParent(nullptr);
                EntityRemovedEvent<EntityType> e(entity);
                eventService(e);
                return true;
            }

            return false;
        }

        iterator begin() {
            return entities.begin();
        }

        iterator end() {
            return entities.end();
        }

        const_iterator begin() const {
            return entities.begin();
        }

        const_iterator end() const {
            return entities.end();
        }

        PAX_NODISCARD bool empty() const {
            return entities.empty();
        }

        PAX_NODISCARD EventService & getEventService() const {
            return eventService;
        }
    };
}

#endif //POLYPROPYLENE_ENTITYMANAGER_H
