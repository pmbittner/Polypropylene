//
// Created by paul on 22.12.19.
//

#ifndef POLYPROPYLENE_PROTOTYPEENTITYPREFAB_H
#define POLYPROPYLENE_PROTOTYPEENTITYPREFAB_H

#include <vector>
#include "Property.h"
#include "EntityPrefab.h"

namespace PAX {
    template<class EntityType>
    class PrototypeEntityPrefab : public EntityPrefab<EntityType> {
        std::vector<Property<EntityType>*> prototypes;

    public:
        explicit PrototypeEntityPrefab(const Entity<EntityType> & prototype) {
            const std::vector<Property<EntityType>*> & prototypeProperties = prototype.getAllProperties();
            for (Property<EntityType> * original : prototypeProperties) {
                prototypes.emplace_back(original->clone());
            }
        }

        PrototypeEntityPrefab(const PrototypeEntityPrefab<EntityType> & other) = default;

        void addMyContentTo(EntityType & entity, const VariableRegister & variableRegister) override {
            for (Property<EntityType> * prototype : prototypes) {
                entity.add(prototype->clone());
            }
        }
    };
}

#endif //POLYPROPYLENE_PROTOTYPEENTITYPREFAB_H
