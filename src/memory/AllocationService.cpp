//
// Created by Paul Bittner on 08.01.2021.
//

#include <polypropylene/memory/AllocationService.h>

namespace PAX {
    AllocationService::AllocationService()
    : allocatorFactory([](const Type & t){
        std::stringstream name;
        name << "[" << t.name() << "]";
        return std::make_shared<PoolAllocator>(name.str(), t.size);
    })
    {}

    AllocationService::~AllocationService() = default;

    void AllocationService::setDefaultAllocatorFactory(const AllocatorFactory &factory) {
        this->allocatorFactory = factory;
    }

    void AllocationService::registerAllocator(const TypeId & type, const std::shared_ptr<Allocator> & allocator) {
        allocators.insert_or_assign(type, allocator);
    }

    std::shared_ptr<Allocator> AllocationService::unregisterAllocator(const TypeId & type) {
        auto iterator = allocators.find(type);
        if (iterator != allocators.end()) {
            std::shared_ptr<Allocator> elementToRemove = std::move(iterator->second);
            allocators.erase(iterator);
            return elementToRemove;
        }

        return nullptr;
    }

    std::shared_ptr<Allocator> AllocationService::getAllocator(const TypeId &type) {
        auto iterator = allocators.find(type);
        if (iterator != allocators.end()) {
            return iterator->second;
        }

        return nullptr;
    }

    bool AllocationService::hasAllocated(const TypeId & t, void * object) const {
        const auto & it = allocators.find(t);
        return it != allocators.end() && it->second->isMine(object);
    }

    void * AllocationService::allocate(const Type & t) {
        std::shared_ptr<Allocator> allocator = nullptr;

        const auto & allocIt = allocators.find(t.id);
        if (allocIt != allocators.end()) {
            allocator = allocIt->second;

            if (allocator->getAllocationSize() != t.size) {
                PAX_THROW_RUNTIME_ERROR("Allocator registered for type " << t.name() << " does not allocate data of size_t " << t.size << "!");
            }
        }

        // Create default allocator
        if (!allocator) {
            allocator = allocatorFactory(t);
            registerAllocator(t.id, allocator);
        }

        return allocator->allocate();
    }

    bool AllocationService::free(const TypeId & type, void * object) {
        const auto& it = allocators.find(type);
        if (it == allocators.end()) {
            PAX_THROW_RUNTIME_ERROR("Cannot free \"" << object << "\" because there is no IAllocator registered for the given type \"" << type.name() << "\"!");
        }

        const auto & allocator = it->second;
        if (allocator->isMine(object)) {
            return allocator->free(object);
        } else {
            PAX_THROW_RUNTIME_ERROR("Cannot free \"" << object << "\" because it was not allocated by the allocator \"" << allocator << "\" registered for the given type \"" << type.name() << "\" in this AllocationService!");
        }
    }
}