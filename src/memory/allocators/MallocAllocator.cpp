//
// Created by Paul Bittner on 09.01.2021.
//

#include <polypropylene/memory/allocators/MallocAllocator.h>

namespace PAX {
    MallocAllocator::MallocAllocator(size_t elementSize)
    : elementSize(elementSize)
    {}

    void * MallocAllocator::allocate() {
        return malloc(elementSize);
    }

    void MallocAllocator::free(void * data) {
        ::free(data);
    }

    size_t MallocAllocator::getAllocationSize() {
        return elementSize;
    }
}