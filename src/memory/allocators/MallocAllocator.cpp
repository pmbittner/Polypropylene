//
// Created by Paul Bittner on 09.01.2021.
//

#include <polypropylene/memory/allocators/MallocAllocator.h>

namespace PAX {
    MallocAllocator::MallocAllocator(const std::string & name, size_t elementSize) :
    Allocator(name),
    elementSize(elementSize)
    {}

    void * MallocAllocator::allocate() {
        void * mem = malloc(elementSize);
        allocatedObjects.insert(mem);
        return mem;
    }

    bool MallocAllocator::free(void * data) {
        if (isMine(data)) {
            allocatedObjects.erase(data);
            ::free(data);
            return true;
        }
        return false;
    }

    size_t MallocAllocator::getAllocationSize() const {
        return elementSize;
    }

    bool MallocAllocator::isMine(void *data) const {
        return allocatedObjects.find(data) != allocatedObjects.end();
    }
}