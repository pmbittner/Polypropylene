//
// Created by Paul Bittner on 08.01.2021.
//

#include <polypropylene/memory/AllocationService.h>

namespace PAX {
    AllocationService::~AllocationService() {
        for (IAllocator * a : defaultAllocators) {
            delete a;
        }
    }

    void AllocationService::registerAllocator(const TypeId & type, IAllocator * allocator) {
        allocators[type] = allocator;
    }

    [[maybe_unused]] IAllocator * AllocationService::unregisterAllocator(const TypeId & type) {
        auto iterator = allocators.find(type);
        if (iterator != allocators.end()) {
            IAllocator * elementToRemove = iterator->second;
            // Remove this allocator from the default allocators if it is a default allocator.
            Util::removeFromVector(defaultAllocators, elementToRemove);
            allocators.erase(iterator);
            return elementToRemove;
        }

        return nullptr;
    }

    bool AllocationService::hasAllocated(void *object) {
        // TODO: Maybe we find a better way for detecting this than caching all pointers in a vector.
        return Util::vectorContains(allocatedObjects, object);
    }

    bool AllocationService::free(const TypeId &type, void *object) {
        const auto& allocator = allocators.find(type);
        if (allocator != allocators.end() && Util::removeFromVector(allocatedObjects, object)) {
            allocator->second->destroy(object);
            return true;
        }

        return false;
    }
}