//
// Created by paul on 06.01.19.
//

#ifndef POLYPROPYLENE_ENTITYPREFAB_H
#define POLYPROPYLENE_ENTITYPREFAB_H

#include "polypropylene/StringVariables.h"
#include "polypropylene/property/construction/PropertyFactory.h"
#include "polypropylene/memory/Allocator.h"

namespace PAX {
    class Prefab {
    public:
        static VariableRegister PreDefinedVariables;
    };

    template<class EntityType>
    class EntityPrefab : public Prefab {
    protected:
        std::map<Path, std::shared_ptr<EntityPrefab<EntityType>>> parentPrefabs;

    public:
        explicit EntityPrefab() = default;
        virtual ~EntityPrefab() = default;
        virtual EntityType * create(const VariableRegister & variableRegister) = 0;
        virtual void addMyContentTo(EntityType& c, const VariableRegister & variableRegister) = 0;
    };
}

#endif //POLYPROPYLENE_ENTITYPREFAB_H
