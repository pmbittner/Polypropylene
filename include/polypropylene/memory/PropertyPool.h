//
// Created by Paul on 13.08.2019.
//

#ifndef POLYPROPYLENE_PROPERTYPOOL_H
#define POLYPROPYLENE_PROPERTYPOOL_H

#include <polypropylene/memory/AllocationService.h>
#include "paxcore/system/System.h"
#include "polypropylene/memory/allocators/PoolAllocator.h"

namespace PAX {
    template<typename PropertyType>
    struct DefaultChunkValidator {
        bool isValid(typename PoolAllocator<sizeof(PropertyType)>::MemoryChunk * chunk) {
            return chunk->allocated;
        }
    };

    /**
     * Iterator for pool allocators used for PropertyOwningSystems.
     * @tparam PropertyType The type of property this iterator iterates over.
     */
    template<typename PropertyType, typename ValidatorType = DefaultChunkValidator<PropertyType>>
    struct PropertyPoolIterator {
    private:
        ValidatorType validator;
        typename PoolAllocator<sizeof(PropertyType)>::MemoryChunk * current = nullptr;

    public:
        explicit PropertyPoolIterator(typename PoolAllocator<sizeof(PropertyType)>::MemoryChunk * pos)
                : current(pos), validator()
        {
            if (!validator.isValid(current)) {
                this->operator++();
            }
        }

        PropertyPoolIterator & operator=(const PropertyPoolIterator & other) = default;

        inline bool operator==(const PropertyPoolIterator & other) {
            return current == other.current;
        }

        inline bool operator!=(const PropertyPoolIterator & other) {
            return !operator==(other);
        }

        PropertyType * operator*() {
            return reinterpret_cast<PropertyType*>(&(current->data));
        }

        PropertyPoolIterator & operator++() {
            do {
                ++current;
            } while (!validator.isValid(current));

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
    template<class PropertyType, class IteratorType = PropertyPoolIterator<PropertyType>>
    class PropertyPool {
        PoolAllocator<sizeof(PropertyType)> allocator;

    public:
        PropertyPool() = default;

        void initialize() {
            AllocationService & allocationService = PropertyType::EntityType::GetPropertyAllocator();
            allocationService.registerAllocator(typeid(PropertyType), &allocator);
        }

        IteratorType begin() { return allocator.getMemory(); }
        IteratorType end() { return allocator.getMemory() + allocator.getCapacity(); }
        //PropertyOwningSystemIterator<PropertyType> begin() const { return Iterator(allocator.getMemory()); }
        //PropertyOwningSystemIterator<PropertyType> end() const { Iterator(allocator.getMemory() + allocator.getCapacity()); }
    };
}

#endif //POLYPROPYLENE_PROPERTYPOOL_H
