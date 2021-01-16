//
// Created by paul on 22.09.18.
//

#ifndef POLYPROPYLENE_PROPERTYALLOCATIONSERVICE_H
#define POLYPROPYLENE_PROPERTYALLOCATIONSERVICE_H

#include <memory>
#include <unordered_set>

#include <polypropylene/stdutils/CollectionUtils.h>
#include "polypropylene/reflection/TypeMap.h"

#include "Allocator.h"
#include "allocators/PoolAllocator.h"

namespace PAX {
    /**
     * Central service for managing allocation and deallocation of arbitrary types.
     * In Polypropylene, the AllocationService is used for allocating and deallocating properties.
     * For each type, a custom allocator can be registered.
     * By default, a PoolAllocator will be registered for each type lazily.
     */
    class AllocationService final {
        TypeMap<std::shared_ptr<IAllocator>> allocators;
        std::unordered_set<void*> allocatedObjects;

    public:
        AllocationService() = default;
        virtual ~AllocationService();

        /**
         * Registers the given allocator for (de-) allocating objects of the given type.
         */
        void registerAllocator(const TypeId & type, const std::shared_ptr<IAllocator> & allocator);

        /**
         * Removes the allocator that is registered for the given type
         * from this AllocationService and returns it.
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
         *         Returns nullptr if this AllocationService does not contain
         *         any allocator for the given type.
         */
        std::shared_ptr<IAllocator> unregisterAllocator(const TypeId & type);

        /**
         * Returns the IAllocator that is registered for the given type.
         * @param type The type for which the allocator should be returned.
         * @return The IAllocator that is registered for the given type.
         *         Returns nullptr if there is no such allocator.
         */
        std::shared_ptr<IAllocator> getAllocator(const TypeId & type);

        /**
         * Returns true iff the given object was allocated with this AllocationService.
         * @param object Pointer to any memory for which you want to know if it was
         *               allocated with this AllocationService or not.
         * @return True iff the given object was allocated with this AllocationService.
         */
        bool hasAllocated(void * object);

        /**
         * Allocates memory for the given type.
         * The returned pointer will be of size 't.size'.
         * This method is designed to be used with placement new.
         * Given some type T then you would use this method as follows:
         *   AllocationService & s = ...;
         *   T * t = new (s.allocate(paxtypeof(T))) T(some, arguments);
         * @tparam T The type for which memory should be allocated.
         * @return A pointer to new memory of size 't.size'.
         */
        PAX_NODISCARD void * allocate(Type t);

        /**
         * Frees the given memory that was allocated used for objects of the given type.
         * This method assumes that the given memory was allocated with this AllocationService.
         * This method does not invoke the destructor of the given memory (because there is no
         * static type information).
         * If you want to delete (i.e., invoke the destructor) and free the memory use deleteAndFree<T> instead.
         * Throws a runtime error with corresponding message if
         *   - there is no allocator registered for the given type in this AllocationService.
         *   - or the given memory was not allocated with this AllocationService.
         * @param type The type for which the given memory was allocated with allocate<T>.
         *             This method will look for the allocator registered for this type in this
         *             AllocationService and assumes that the given memory was allocated with the
         *             found allocator.
         * @param memory The memory that should be freed.
         */
        void free(const TypeId & type, void * memory);

        /**
         * Deletes the given object by invoking its constructor.
         * Frees its memory afterwards by invoking `free`.
         * Does nothing if the given object was not allocated trough this allocation service.
         * @tparam DestructorType The type of the constructor that should be invoked (~DestructorType()).
         * @param t The object which will be deleted.
         * @param type The type for which the given memory was allocated with allocate<T>.
         *             This method will look for the allocator registered for this type in this
         *             AllocationService and assumes that the given memory was allocated with the
         *             found allocator.
         * @return True iff the object was deleted.
         *         False iff the object was not allocated with this
         *         AllocationService and thus could not be freed.
         */
        template<typename DestructorType>
        bool deleteAndFree(DestructorType * t, const TypeId & type) {
            if (hasAllocated(t)) {
                t->~DestructorType();
                free(type, t);
                return true;
            }
            return false;
        }

        template<typename DestructorType>
        bool deleteAndFree(DestructorType * t) {
            return deleteAndFree(t, paxtypeid(DestructorType));
        }
    };
}
#endif //POLYPROPYLENE_PROPERTYALLOCATIONSERVICE_H
