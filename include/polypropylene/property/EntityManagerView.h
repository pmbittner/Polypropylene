//
// Created by Paul on 23.03.2019.
//

#ifndef POLYPROPYLENE_PROPERTYSYSTEM_H
#define POLYPROPYLENE_PROPERTYSYSTEM_H

#include "EntityManager.h"

namespace PAX {
    /**
     * EntityManagerViews filter the entities of an EntityManager by their containing properties.
     * The method EntityManagerView::getEntities() returns exactly those entities from the given manager that contain
     * the specified properties.
     *
     * @tparam EntityType The concrete Entity type (i.e., the derived class)
     * @tparam RequiredProperties A list of Property types that should be contained by filtered entities.
     */
    template<typename EntityType, typename... RequiredProperties>
    class EntityManagerView {
        const EntityManager<EntityType> & manager;

        /*
         * Currently we are using a sorted set::vector here to optimize for iteration time.
         * According to this (non-professional?) benchmark
         * https://tinodidriksen.com/2010/04/cpp-set-performance/
         * using a sorted vector is an order of magnitude faster than iterating a set.
         * Insertions and deletions are faster for sets but the difference is less big.
         *
         * https://stackoverflow.com/questions/2710221/is-there-a-sorted-vector-class-which-supports-insert-etc
         * Boost's flat_set might be a good alternative but I want to avoid introducing such a dependency.
         * I cannot add just the file on its own to this project.
         *
         * Prove me wrong and open a pull-request if you have evidence that set is better. :)
         */
        std::vector<EntityType*> entities;

    public:
        using iterator = typename decltype(entities)::iterator;
        using const_iterator = typename decltype(entities)::const_iterator;

    private:
        bool isValid(EntityType * entity) {
            return entity->template has<RequiredProperties...>();
        }

        /**
         * Returns an iterator to the given entity in the entities vector.
         * If the entity is not yet in the vector, returns the position where
         * the entities has to be inserted to keep the vector sorted.
         */
        const_iterator find(EntityType * entity) const {
            if (entities.empty()) {
                return entities.begin();
            }

            int64_t first = 0;
            int64_t last = entities.size() - 1;
            // inserted sorted by pointer to improve caching on iterating
            while (first < last) {
                int64_t mid = (first + last) / 2;
                EntityType * entityInMid = entities.at(mid);
                if (entityInMid == entity) {
                    first = mid;
                    break;
                } else if (entityInMid < entity) {
                    first = mid + 1;
                } else { // entity < entityInMid
                    last  = mid - 1;
                }
            }
            return entities.begin() + first;
        }

        void tryAdd(EntityType * entity) {
            if (isValid(entity)) {
                const const_iterator it = find(entity);
                if (entities.empty() || *it != entity) { // If the given entity is not already in the vector
                    entities.insert(it, entity);
                }
            }
        }

        void remove(EntityType * entity) {
            const const_iterator it = find(entity);
            const bool isRightEntity = entity == *it;
            if (isRightEntity) {
                entities.erase(it);
            }
//                entities.erase(entity); // when using set
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
        // TODO: Delete this, too? All event service pointers will invalide on move right?
//        EntityManagerView(EntityManagerView<EntityType, RequiredProperties...> && other) noexcept = default;

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
            remove(e.entity);
        }

        template<typename Prop>
        void onPropertyAttached(PropertyAttachedEvent<EntityType, Prop> & e) {
            tryAdd(e.entity);
        }

        template<typename Prop>
        void onPropertyDetached(PropertyDetachedEvent<EntityType, Prop> & e) {
            if (!isValid(e.entity)) {
                remove(e.entity);
            }
        }

        PAX_NODISCARD const std::vector<EntityType*> & getEntities() const {
            return entities;
        }

        PAX_NODISCARD size_t size() const noexcept {
            return entities.size();
        }

        iterator begin() { return entities.begin(); }
        iterator end() { return entities.end(); }
        const_iterator begin() const { return entities.begin(); }
        const_iterator end() const { return entities.end(); }
    };
}

#endif //POLYPROPYLENE_PROPERTYSYSTEM_H
