//
// Created by paul on 06.01.19.
//

#ifndef POLYPROPYLENE_ENTITYPREFAB_H
#define POLYPROPYLENE_ENTITYPREFAB_H

#include "polypropylene/reflection/VariableRegister.h"
#include "PropertyFactory.h"
#include "polypropylene/memory/Allocator.h"

namespace PAX {
    class Prefab {
    public:
        /**
         * Allows specifying global variables for all prefabs.
         * For example, the working directory of the current program could be made accessible to all prefabs this way.
         */
        static VariableRegister PreDefinedVariables;
    };

    /**
     * Prefabs create new entities with a pre-defined set of properties.
     * Each entity created by an prefab can be considered equal.
     * Prefabs my inherit from each other when supported by the concrete prefab implementation.
     *
     * @tparam EntityType The concrete Entity type (i.e., the derived class)
     */
    template<class EntityType>
    class EntityPrefab : public Prefab {
    protected:
        std::map<Path, std::shared_ptr<EntityPrefab<EntityType>>> parentPrefabs;

    public:
        explicit EntityPrefab() = default;
        virtual ~EntityPrefab() = default;

        /**
         * Creates a new entity from this prefab.
         * @param variableRegister Custom variables that can be considered upon creation.
         * @return returns a new entity of this prefab.
         */
        virtual EntityType * create(const VariableRegister & variableRegister) = 0;

        /**
         * Adds all properties of this prefab to the given entity.
         * This can be used to combine multiple prefabs.
         * @param entity The entity to which the properties of this prefab should be added to.
         * @param variableRegister Custom variables that can be considered upon creation.
         */
        virtual void addMyContentTo(EntityType & entity, const VariableRegister & variableRegister) = 0;
    };
}

#endif //POLYPROPYLENE_ENTITYPREFAB_H
