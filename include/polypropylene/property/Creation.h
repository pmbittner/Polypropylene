//
// Created by Paul Bittner on 10.01.2021.
//

#ifndef POLYPROPYLENE_CREATION_H
#define POLYPROPYLENE_CREATION_H

#include "polypropylene/definitions/Definitions.h"
#include "polypropylene/reflection/Polymorphic.h"

/**
 * Convenience macro for creating properties and entities with the allocation service.
 * Use it as follows:
 *    Cheese * c = pax_new(Cheese)(arguments, for, constructor, here);
 * @param propOrEntityType The type of the property or entity you want to create.
 * @see pax_delete
 */
#define pax_new(propOrEntityType) \
    new (propOrEntityType::EntityType::GetAllocationService().allocate(paxtypeof(propOrEntityType))) propOrEntityType

/**
 * Convenience function for easy deletion of properties and entities that were
 * allocated with the AllocationService (e.g., with pax_new).
 * This is a shortcut to invoking AllocationService::deleteAndFree<T>.
 * @tparam T The type on which the destructor for t should be called.
 *           Has to implement the Polymorphic interface.
 * @param t Pointer to the object that should be deleted.
 * @return True iff the object was deleted.
 *         False iff the object was not allocated with the
 *         AllocationService and thus could not be deleted and freed.
 * @see pax_new, AllocationService::deleteAndFree<T>
 */
template <class T>
typename std::enable_if<std::is_base_of<::PAX::Polymorphic, T>::value, bool>::type
pax_delete(T * t) {
    return T::EntityType::GetAllocationService().template deleteAndFree<T>(t, t->getClassType().id);
}

#endif //POLYPROPYLENE_CREATION_H
