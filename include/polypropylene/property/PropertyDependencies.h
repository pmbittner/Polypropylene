//
// Created by paul on 22.09.18.
//

#ifndef POLYPROPYLENE_PROPERTYDEPENDENCIES_H
#define POLYPROPYLENE_PROPERTYDEPENDENCIES_H

#include "../Dependency.h"
#include "ForwardDeclarations.h"

namespace PAX {
    template<class EntityType, class... Properties>
    class PropertyDependencies : public Dependency<EntityType> {
    public:
        PAX_NODISCARD bool met(const EntityType & entity) const override {
            return entity.template has<Properties...>();
        }
    };

    // PropertyContainer::has always needs arguments (see implementation of met)
    template<class EntityType>
    class PropertyDependencies<EntityType> : public Dependency<EntityType> {

    };
}
#endif //POLYPROPYLENE_PROPERTYDEPENDENCIES_H
