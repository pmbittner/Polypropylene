//
// Created by Paul on 13.08.2019.
//

#ifndef POLYPROPYLENE_PROPERTYOWNINGSYSTEM_H
#define POLYPROPYLENE_PROPERTYOWNINGSYSTEM_H

#include <polypropylene/memory/AllocationService.h>
#include "System.h"
#include "../memory/allocators/PoolAllocator.h"

namespace PAX {
    /**
     * Iterator for pool allocators used for PropertyOwningSystems.
     * @tparam PropertyType The type of property this iterator iterates over.
     */
    template<typename PropertyType>
    struct PropertyOwningSystemIterator {
    private:
        typename PoolAllocator<sizeof(PropertyType)>::MemoryChunk * current = nullptr;

        inline bool isCurrentValid() {
            return current->allocated && reinterpret_cast<PropertyType*>(&(current->data))->isActive();
        }

    public:
        explicit PropertyOwningSystemIterator(typename PoolAllocator<sizeof(PropertyType)>::MemoryChunk * pos)
                : current(pos)
        {
            if (!isCurrentValid()) {
                this->operator++();
            }
        }

        PropertyOwningSystemIterator & operator=(const PropertyOwningSystemIterator & other) = default;

        inline bool operator==(const PropertyOwningSystemIterator & other) {
            return current == other.current;
        }

        inline bool operator!=(const PropertyOwningSystemIterator & other) {
            return !operator==(other);
        }

        PropertyType * operator*() {
            return reinterpret_cast<PropertyType*>(&(current->data));
        }

        PropertyOwningSystemIterator & operator++() {
            do {
                ++current;
            } while (!isCurrentValid());

            return *this;
        }
    };

    /**
     * Owning systems own all properties of a given type and manage their allocation.
     * Iterating over the system yields all active properties that are currently in use.
     * Note that only properties allocated via the allocation service of the corresponding entity
     * (@ref Entity<T>::GetPropertyAllocator()) are owned by and known to this system.
     * @tparam ManagerType The manager type to which this system can be added to.
     * @tparam PropertyType The type of properties that should be allocated by this system.
     * For the specified property type a pool allocator is registered in the allocation service of the corresponding
     * entity.
     */
    template<class ManagerType, class PropertyType>
    class PropertyOwningSystem : public System<ManagerType> {
        PoolAllocator<sizeof(PropertyType)> allocator;

    public:
        PropertyOwningSystem() = default;

        void initialize(ManagerType * manager) override {
            System<ManagerType>::initialize(manager);
            AllocationService & allocationService = PropertyType::EntityType::GetPropertyAllocator();
            allocationService.registerAllocator(typeid(PropertyType), &allocator);
        }

        PropertyOwningSystemIterator<PropertyType> begin() { return allocator.getMemory(); }
        PropertyOwningSystemIterator<PropertyType> end() { return allocator.getMemory() + allocator.getCapacity(); }
        //PropertyOwningSystemIterator<PropertyType> begin() const { return Iterator(allocator.getMemory()); }
        //PropertyOwningSystemIterator<PropertyType> end() const { Iterator(allocator.getMemory() + allocator.getCapacity()); }
    };
}

#endif //POLYPROPYLENE_PROPERTYOWNINGSYSTEM_H
