//
// Created by paul on 06.01.19.
//

#ifndef POLYPROPYLENE_PROPERTYCONTAINERPREFAB_H
#define POLYPROPYLENE_PROPERTYCONTAINERPREFAB_H

#include "polypropylene/StringVariables.h"
#include "polypropylene/property/construction/PropertyFactory.h"
#include "polypropylene/memory/Allocator.h"

namespace PAX {
    class Prefab {
    public:
        static VariableRegister PreDefinedVariables;
    };

    template<class C>
    class PropertyContainerPrefab : public Prefab {
    protected:
        std::map<Path, std::shared_ptr<PropertyContainerPrefab<C>>> parentPrefabs;

    public:
        explicit PropertyContainerPrefab() = default;
        virtual ~PropertyContainerPrefab() = default;
        virtual C * create(const VariableRegister & variableRegister) = 0;
        virtual void addMyContentTo(C& c, const VariableRegister & variableRegister) = 0;
    };
}

#endif //POLYPROPYLENE_PROPERTYCONTAINERPREFAB_H
