//
// Created by paul on 06.01.20.
//

#ifndef POLYPROPYLENE_CLONE_H
#define POLYPROPYLENE_CLONE_H

#include "Entity.h"
#include "PropertyFactory.h"

namespace PAX {
    /**
     * Creates a copy of the given property by considering all fields specified by the @ref getMetadata() method.
     * @param propertyToClone The property that should be cloned.
     * @return A clone of this property.
     * @tparam TEntityType The concrete type of Entity, to which the given property belongs.
     */
    template<typename TEntityType>
    PAX_NODISCARD static typename TEntityType::PropertyType * Clone(typename TEntityType::PropertyType * propertyToClone) {
        // Theoretically, the return type does not have to be TEntityType::PropertyType.
        // However, it is the only return type making sense because no other types of properties can be added to TEntityType.
        typename TEntityType::PropertyType * clone
            = PropertyFactoryRegister<TEntityType>::getFactoryFor(propertyToClone->getClassType().id)->create();
        ClassMetadata cloneMetadata = clone->getMetadata();
        propertyToClone->getMetadata().writeTo(cloneMetadata);
        clone->created();
        return clone;
    }
}

#endif //POLYPROPYLENE_CLONE_H
