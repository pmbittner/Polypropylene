//
// Created by paul on 22.12.19.
//

#ifndef POLYPROPYLENE_PROTOTYPEENTITYPREFAB_H
#define POLYPROPYLENE_PROTOTYPEENTITYPREFAB_H

#include <vector>
#include "polypropylene/prefab/Prefab.h"
#include "Clone.h"

namespace PAX {
    template<class TEntityType>
    class PrototypeEntityPrefab : public Prefab<TEntityType> {
        using PropertyType = typename TEntityType::PropertyType;
        std::vector<PropertyType *> prototypes;

    public:
        explicit PrototypeEntityPrefab(const TEntityType & prototype) {
            const std::vector<PropertyType *> & prototypeProperties = prototype.getAllProperties();
            for (PropertyType * original : prototypeProperties) {
                prototypes.emplace_back(Clone<TEntityType>(original));
            }
        }

        ~PrototypeEntityPrefab() override {
            for (Property<TEntityType> * p : prototypes) {
                pax_delete(p);
            }
        }

        PrototypeEntityPrefab(const PrototypeEntityPrefab<TEntityType> & other) = default;

        void addMyContentTo(TEntityType & entity, const VariableRegister & variableRegister) override {
            for (PropertyType * prototype : prototypes) {
                PropertyType * copy = Clone<TEntityType>(prototype);
                entity.add(copy);
            }
        }
    };

    template<class TDerived, class TPropertyType>
    PrototypeEntityPrefab<TDerived> Entity<TDerived, TPropertyType>::toPrefab() const {
        return PrototypeEntityPrefab<TDerived>(*static_cast<const TDerived*>(this));
    }
}

#endif //POLYPROPYLENE_PROTOTYPEENTITYPREFAB_H
