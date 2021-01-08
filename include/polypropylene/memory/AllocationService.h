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
    class AllocationService {
        TypeMap<IAllocator*> allocators;
        std::vector<IAllocator*> defaultAllocators;
        std::vector<void*> allocatedObjects;

    public:
        AllocationService() = default;
        virtual ~AllocationService();

        /**
         * Registers the given allocator for (de-) allocating objects of the given type.
         * Does not take ownership.
         */
        void registerAllocator(const TypeId & type, IAllocator * allocator);

        /**
         * Removes the allocator that is registered for the given type
         * from this AllocationService and returns it.
         * Transfers ownership to the caller if the allocator was a default allocator.
         * Otherwise, the allocator is owned by some other object different from this
         * AllocationService.
         *
         * Beware! This is a very dangerous call that can lead to memory leaks
         * and segmentation faults if not used carefully.
         * Notice, that there might be objects around that were allocated with the
         * allocator you are about to remove.
         * Deleting those objects might be impossible afterwards because the
         * AllocationService does not know the proper allocator for freeing those
         * objects anymore because it was removed.
         * So remove an allocator only, if there are no objects allocated with
         * it around anymore!
         *
         * @param type The type for which the current allocator should be removed.
         * @return The removed allocator.
         *         Ownership might transfer to the owner if this was one of
         *         the default allocators.
         *         Returns nullptr if this AllocationService does not contain
         *         any allocator for the given type.
         */
        IAllocator * unregisterAllocator(const TypeId & type);

        /**
         * Returns true iff the given object was allocated with this AllocationService.
         * @param object Pointer to any memory for which you want to know if it was
         *               allocated with this AllocationService or not.
         * @return True iff the given object was allocated with this AllocationService.
         */
        bool hasAllocated(void * object);

        /**
         * Allocates memory for the given type.
         * The returned pointer will be of size sizeof(T).
         * This method is designed to be used with placement new.
         *   AllocationService * s = ...;
         *   T * t = new (s->allocate<T>()) T(some, arguments);
         * @tparam T The type for which memory should be allocated.
         * @return A pointer to new memory of size sizeof(T).
         */
        template<class T>
        PAX_NODISCARD void * allocate() {
            // TODO: Can we avoid the template of this method?
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

        /**
         * Frees the given memory that was allocated used for objects of the given type.
         * This method assumes that the given memory was allocated with this AllocationService.
         * This method does not invoke the destructor of the given memory (because there is no
         * static type information).
         * If you want to delete (i.e., invoke the destructor) and free the memory use deleteAndFree<T> instead.
         * @param type The type for which the given memory was allocated with allocate<T>.
         *             This method will look for the allocator registered for this type in this
         *             AllocationService and assumes that the given memory was allocated with the
         *             found allocator.
         * @param memory The memory that should be freed.
         * @return True iff the given memory could be freed.
         *         Returns false, when
         *         - There is no allocator registered for the given type in this AllocationService.
         *         - or the given memory was not allocated with this AllocationService.
         */
        bool free(const TypeId & type, void * memory);

        /**
         * Deletes the given object by invoking its constructor.
         * Frees its memory afterwards by invoking `free`.
         * @tparam T The type of the constructor that should be invoked (~T()).
         * @param t The object which will be deleted.
         * @return True iff the memory of the given object could be free.
         *         False if the inner call to `free` failed.
         *         Then, t is a zombie afterwards, meaning its memory is still allocated
         *         but the object was destroyed.
         */
        template<typename T>
        bool deleteAndFree(T * t) {
            t->~T();
            return free(paxtypeid(T), t);
        }
    };
}
#endif //POLYPROPYLENE_PROPERTYALLOCATIONSERVICE_H
