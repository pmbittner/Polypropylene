//
// Created by Paul Bittner on 10.01.2021.
//

#include "polypropylene/memory/PropertyPool.h"

namespace PAX {
    PAX_MAYBEUNUSED bool DefaultChunkValidator::isValid(const PoolAllocator &pool, PoolAllocator::Index i) const {
        return
                0 <= i && i < pool.getCapacity() &&
                pool.getChunkInfo(i)->allocated;
    }
}