//
// Created by Paul on 13.08.2019.
//

#ifndef PAXENGINE3_PROPERTYOWNINGSYSTEM_H
#define PAXENGINE3_PROPERTYOWNINGSYSTEM_H

#include <polypropylene/memory/AllocationService.h>
#include "System.h"
#include "../memory/allocators/PoolAllocator.h"

namespace PAX {
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


    template<class ManagerType, class PropertyType>
    class PropertyOwningSystem : public System<ManagerType> {
        PoolAllocator<sizeof(PropertyType)> allocator;

    public:
        PropertyOwningSystem() = default;

        void initialize(ManagerType *game) override {
            System<ManagerType>::initialize(game);

            AllocationService & allocationService = PropertyType::Container::GetPropertyAllocator();
            allocationService.registerAllocator(paxtypeid(PropertyType), &allocator);
        }

        PropertyOwningSystemIterator<PropertyType> begin() { return allocator.getMemory(); }
        PropertyOwningSystemIterator<PropertyType> end() { return allocator.getMemory() + allocator.getCapacity(); }
        //PropertyOwningSystemIterator<PropertyType> begin() const { return Iterator(allocator.getMemory()); }
        //PropertyOwningSystemIterator<PropertyType> end() const { Iterator(allocator.getMemory() + allocator.getCapacity()); }
    };
}

#endif //PAXENGINE3_PROPERTYOWNINGSYSTEM_H
