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

    void * AllocationService::allocate(TypeHandle t) {
        std::shared_ptr<IAllocator> allocator = nullptr;

        const auto & allocIt = allocators.find(t.id);
        if (allocIt != allocators.end()) {
            allocator = allocIt->second;

            if (allocator->getAllocationSize() != t.size) {
                PAX_THROW_RUNTIME_ERROR("IAllocator registered for type " << t.id.name() << " does not allocate data of size_t " << t.size << "!");
            }
        }

        if (!allocator) {
            allocator = std::make_shared<PoolAllocator>(t.size);
            registerAllocator(t.id, allocator);
        }

        void * mem = allocator->allocate();
        allocatedObjects.push_back(mem);
        return mem;
    }

    void AllocationService::free(const TypeId &type, void * object) {
        const auto& allocator = allocators.find(type);

        if (allocator == allocators.end()) {
            PAX_THROW_RUNTIME_ERROR("Cannot free \"" << object << "\" because there is no IAllocator registered for the given type \"" << type.name() << "\"!");
        }

        if (Util::removeFromVector(allocatedObjects, object)) {
            allocator->second->free(object);
        } else {
            PAX_THROW_RUNTIME_ERROR("Cannot free \"" << object << "\" because it was not allocated by the allocator \"" << allocator->second << "\" registered for the given type \"" << type.name() << "\" in this AllocationService!");
        }
    }
}