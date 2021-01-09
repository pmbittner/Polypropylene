//
// Created by Paul Bittner on 09.01.2021.
//

#include <polypropylene/memory/allocators/MallocAllocator.h>

namespace PAX {
    MallocAllocator::MallocAllocator(size_t chunksize)
    : chunksize(chunksize)
    {}

    void * MallocAllocator::allocate() {
        return malloc(chunksize);
    }

    void MallocAllocator::destroy(void * data) {
        free(data);
    }

    size_t MallocAllocator::getAllocationSize() {
        return chunksize;
    }
}