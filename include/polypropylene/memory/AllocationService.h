//
// Created by paul on 22.09.18.
//

#ifndef POLYPROPYLENE_PROPERTYALLOCATIONSERVICE_H
#define POLYPROPYLENE_PROPERTYALLOCATIONSERVICE_H


#include <polypropylene/stdutils/CollectionUtils.h>
#include "polypropylene/reflection/TypeMap.h"

#include "Allocator.h"
#include "allocators/MallocAllocator.h"
#include "allocators/PoolAllocator.h"

namespace PAX {
    /**
     * Central service for managing allocation and deallocation of arbitrary types.
     * In Polypropylene, the AllocationService is used for allocating and deallocating properties.
     * For each type, a custom allocator can be registered.
     * By default, a PoolAllocator will be registered for each type lazily.
     */
    // TODO: Create a source file.
    class AllocationService {
        TypeMap<IAllocator*> allocators;
        std::vector<IAllocator*> defaultAllocators;
        std::vector<void*> allocatedObjects;

    public:
        AllocationService() = default;

        virtual ~AllocationService() {
            for (IAllocator * a : defaultAllocators) {
                delete a;
            }
        }

        /**
         * Registers the given allocator for (de-) allocating objects of the given type.
         * Does not take ownership.
         */
        void registerAllocator(const TypeId & type, IAllocator * allocator) {
            allocators[type] = allocator;
        }

        /**
         * Removes the allocator that is registered for the given type
         * from this AllocationService and returns it.
         * @param type The type for which the current allocator should be removed.
         * @return The removed allocator.
         *         Ownership might transfer to the owner if this was one of
         *         the default allocators.
         *         Returns nullptr if this AllocationService does not contain
         *         any allocator for the given type.
         */
        IAllocator * unregisterAllocator(const TypeId & type) {
            auto iterator = allocators.find(type);
            if (iterator != allocators.end()) {
                IAllocator * elementToRemove = iterator->second;
                allocators.erase(iterator);
                return elementToRemove;
            }

            return nullptr;
        }

        bool hasAllocated(void * object) {
            // TODO: Maybe we find a better way for detecting this than caching all pointers in a vector.
            return Util::vectorContains(allocatedObjects, object);
        }

        // TODO: Can we avoid the template here in a safe way?
        template<class T>
        PAX_NODISCARD void * allocate() {
            Allocator<sizeof(T)> * allocator = nullptr;

            const auto & allocIt = allocators.find(paxtypeid(T));
            if (allocIt != allocators.end()) {
                allocator = dynamic_cast<Allocator<sizeof(T)>*>(allocIt->second);
                if (!allocator) {
                    PAX_LOG(PAX::Log::Level::Error, "Registered Allocator for " << typeid(T).name() << " is not of type Allocator<" << typeid(T).name() << ">!");
                }
            }

            if (!allocator) {
                allocator = new PoolAllocator<sizeof(T)>();
                defaultAllocators.push_back(allocator);
                registerAllocator(paxtypeid(T), allocator);
            }

            void * mem = allocator->allocate();
            allocatedObjects.push_back(mem);
            return mem;
        }

        bool free(const TypeId & type, void * object) {
            const auto& allocator = allocators.find(type);
            if (allocator != allocators.end() && Util::removeFromVector(allocatedObjects, object)) {
                allocator->second->destroy(object);
                return true;
            }

            return false;
        }

        template<typename T>
        bool deleteAndFree(T * t) {
            t->~T();
            return free(paxtypeid(T), t);
        }
    };
}
#endif //POLYPROPYLENE_PROPERTYALLOCATIONSERVICE_H
