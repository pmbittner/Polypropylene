//
// Created by Paul Bittner on 08.01.2021.
//

#include <polypropylene/memory/AllocationService.h>

namespace PAX {
    AllocationService::~AllocationService() = default;

    void AllocationService::registerAllocator(const TypeId & type, const std::shared_ptr<IAllocator> & allocator) {
        allocators.insert_or_assign(type, allocator);
    }

    std::shared_ptr<IAllocator> AllocationService::unregisterAllocator(const TypeId & type) {
        auto iterator = allocators.find(type);
        if (iterator != allocators.end()) {
            std::shared_ptr<IAllocator> elementToRemove = std::move(iterator->second);
            allocators.erase(iterator);
            return elementToRemove;
        }

        return nullptr;
    }

    std::shared_ptr<IAllocator> AllocationService::getAllocator(const TypeId &type) {
        auto iterator = allocators.find(type);
        if (iterator != allocators.end()) {
            return iterator->second;
        }

        return nullptr;
    }

    bool AllocationService::hasAllocated(void * object) {
        // TODO: Maybe we find a better way for detecting this than caching all pointers in a vector.
        return Util::vectorContains(allocatedObjects, object);
    }

    bool AllocationService::free(const TypeId &type, void * object) {
        const auto& allocator = allocators.find(type);
        if (allocator != allocators.end() && Util::removeFromVector(allocatedObjects, object)) {
            allocator->second->free(object);
            return true;
        }

        return false;
    }
}