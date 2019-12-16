//
// Created by Paul on 23.03.2019.
//

#ifndef POLYPROPYLENE_PROPERTYSYSTEM_H
#define POLYPROPYLENE_PROPERTYSYSTEM_H

#include "EntityManager.h"

namespace PAX {
    template<typename EntityType, typename... RequiredProperties>
    class EntityManagerView {
        const EntityManager<EntityType> & manager;
        std::set<EntityType*> entities;

        bool isValid(EntityType * entity) {
            return entity->template has<RequiredProperties...>();
        }

        void tryAdd(EntityType * entity) {
            if (isValid(entity))
                entities.insert(entity);
        }

        void tryRemove(EntityType * entity) {
            if (!isValid(entity))
                entities.erase(entity);
        }

        template<bool add, typename T>
        void unfoldPropertyEventListeners(EventService & e) {
            PAX_CONSTEXPR_IF (add) {
                e.add<PropertyAttachedEvent<EntityType, T>, EntityManagerView, &EntityManagerView::onPropertyAttached>(this);
                e.add<PropertyDetachedEvent<EntityType, T>, EntityManagerView, &EntityManagerView::onPropertyDetached>(this);
            } else {
                e.remove<PropertyAttachedEvent<EntityType, T>, EntityManagerView, &EntityManagerView::onPropertyAttached>(this);
                e.remove<PropertyDetachedEvent<EntityType, T>, EntityManagerView, &EntityManagerView::onPropertyDetached>(this);
            }
        }

        template<bool add, typename T, typename T2, typename... Others>
        void unfoldPropertyEventListeners(EventService & e) {
            unfoldPropertyEventListeners<add, T>(e);
            unfoldPropertyEventListeners<add, T2>(e);
            unfoldPropertyEventListeners<add, Others...>(e);
        }

    public:
        explicit EntityManagerView(const EntityManager<EntityType> & manager) : manager(manager) {
            for (EntityType * entity : manager.getEntities()) {
                tryAdd(entity);
            }

            manager.getEventService().template add<EntityAddedEvent<EntityType>, EntityManagerView, &EntityManagerView::onEntityAdded>(this);
            manager.getEventService().template add<EntityRemovedEvent<EntityType>, EntityManagerView, &EntityManagerView::onEntityRemoved>(this);

            unfoldPropertyEventListeners<true, RequiredProperties...>(manager.getEventService());
        }

        explicit EntityManagerView(const EntityManagerView<EntityType, RequiredProperties...> & other) = delete;
        EntityManagerView(EntityManagerView<EntityType, RequiredProperties...> && other) noexcept = default;

        virtual ~EntityManagerView() {
            // It is unnecessary to remove all entities by hand.

            manager.getEventService().template remove<EntityAddedEvent<EntityType>, EntityManagerView, &EntityManagerView::onEntityAdded>(this);
            manager.getEventService().template remove<EntityRemovedEvent<EntityType>, EntityManagerView, &EntityManagerView::onEntityRemoved>(this);

            unfoldPropertyEventListeners<false, RequiredProperties...>(manager.getEventService());
        }

        void onEntityAdded(EntityAddedEvent<EntityType> & e) {
            tryAdd(e.entity);
        }

        void onEntityRemoved(EntityRemovedEvent<EntityType> & e) {
            tryRemove(e.entity);
        }

        template<typename Prop>
        void onPropertyAttached(PropertyAttachedEvent<EntityType, Prop> & e) {
            tryAdd(e.entity);
        }

        template<typename Prop>
        void onPropertyDetached(PropertyDetachedEvent<EntityType, Prop> & e) {
            tryRemove(e.entity);
        }

        PAX_NODISCARD const std::set<EntityType*> & getEntities() const {
            return entities;
        }
    };
}

#endif //POLYPROPYLENE_PROPERTYSYSTEM_H
