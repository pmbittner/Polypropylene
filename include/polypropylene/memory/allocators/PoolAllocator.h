//
// Created by Bittner on 14/08/2019.
//

#ifndef POLYPROPYLENE_POOLALLOCATOR_H
#define POLYPROPYLENE_POOLALLOCATOR_H

#include "../Allocator.h"
#include <polypropylene/log/Errors.h>
#include <cstring>

namespace PAX {
    /**
     * A PoolAllocator of fixed capacity.
     * It can neither grow nor shrink.
     */
    class PoolAllocator : public Allocator {
    public:
        using Index = int32_t;

        /**
         * Metadata for each allocatable element.
         */
        struct ChunkInfo {
        public:
            bool allocated;
        };

    private:
        static size_t DefaultCapacity; // = 1024

        /// Chunk data
        using memunit = char;
        static constexpr size_t MetaDataSize = sizeof(ChunkInfo);

        /**
         * This is a stack to remember which chunks of memory are free.
         * The stack is realised as an array where the top is at the beginning and is moved to the back as elements get popped.
         * Hence, when an element is pushed, topIndex is decremented.
         */
        struct IndexStack {
        private:
            const Index capacity;
            Index * stack;
            Index topIndex;

        public:
            IndexStack(Index capacity);
            IndexStack(IndexStack && other) noexcept;
            ~IndexStack();

            Index pop();
            void push(Index val);
            void clear();

            bool empty() const;
            bool full() const;
        } freeChunks;

        /// Memory
        const Index capacity;
        const size_t elementSize;
        Index firstElement = 0;
        Index lastElement = -1;

        /**
         * The memory where everything is stored.
         * This is a sequence of imaginary chunks where each chunk
         * consists of some metadata i followed by user data:
         *
         *     --------------------------------------------
         * <-- | i | userdata | i | userdata | i | userdata -->
         *     --------------------------------------------
         *
         * The metadata i is of size 'MetaDataSize' and is defined by the type ChunkInfo.
         * The userdata is of size 'elementSize'.
         */
        memunit * memory = nullptr;
        int32_t numberOfAllocations = 0;

        PAX_NODISCARD size_t ChunkSize() const;
        PAX_NODISCARD size_t MemorySize() const;

        /**
         * @return The user data that is associated to the given metadata.
         */
        PAX_NODISCARD static void * DataOf(ChunkInfo * chunk);

        /**
         * @return A pointer into 'memory' to the beginning of this data's chunk
         * (w.r.t. to its prefixing ChunkInfo).
         */
        PAX_NODISCARD static memunit * FromData(void * data);

        /**
         * Assumes that the given pointer points to the begin of a data chunk in 'memory'
         * (i.e., (chunk - memory) % ChunkSize() == 0).
         * @return The metadata for the data chunk at the given pointer into 'memory'.
         */
        PAX_NODISCARD ChunkInfo* InfoFor(memunit * chunk) const;

        /**
         * Assumes that the given pointer points to the begin of a data chunk in 'memory'
         * (i.e., (chunk - memory) % ChunkSize() == 0).
         * @return The index of the data chunk at the given pointer into 'memory'.
         */
        PAX_NODISCARD Index indexOf(const memunit * m) const;

        /**
         * @return A pointer to the data chunk inside 'memory' at the given index.
         */
        PAX_NODISCARD memunit* memAtIndex(Index index) const;

        /**
         * Resets the bounds signaling that no elements are allocated at all.
         * Only call this, when the pool is empty or emptied!
         * Otherwise iterating the pool will result in wrong behaviour!
         */
        void clearBounds();

        /**
         * Updates the bounds assuming the element at index i was just deleted.
         * In particular, checks whether i was the first or the last element and
         * updates the bounds accordingly.
         * @param i Index of the element that was just deleted.
         */
        void updateBoundsAfterDeletionOf(Index i);

    public:
        /**
         * Creates a PoolAllocator of a fixed capacity.
         * @param name The name of this allocator used for debug messages.
         * @param elementSize The size each allocated data object should have.
         * @param capacity The maximum number of elements that can be allocated simultaneously.
         */
        PoolAllocator(const std::string & name, size_t elementSize, Index capacity = DefaultCapacity);
        PoolAllocator(PAX::PoolAllocator && other) noexcept;

        PoolAllocator(const PoolAllocator & other) = delete;
        PoolAllocator & operator=(const PoolAllocator & other) = delete;

        ~PoolAllocator() override;

        /**
         * Allocates a new chunk of data in the pool and returns it.
         * This reduces the available chunks to allocate by 1.
         * If already n chunks are allocated where n is the capacity of this PoolAllocator,
         * a runtime_error with message "memory overflow" will be thrown.
         * The caller will get ownership of the returned memory chunk.
         * The memory can be given back to the PoolAllocator with the free method.
         * The size of the returned memory is given by getAllocationSize().
         * @return Pointer to the chunk that was allocated. It is now owned by the caller.
         */
        PAX_NODISCARD void * allocate() override;

        /**
         * Gives ownership of the given memory back to this PoolAllocator.
         * The memory is not altered right away but available for reallocation
         * (i.e., upcoming calls to allocate() can return the given chunk again).
         * So beware of the following:
         *   - accessing the given pointer after calling this method results in undefined behaviour
         *   - accessing the given pointer can lead to ghost bugs because when accessing the memory
         *     it might already be in use again (as it was allocated again somewhere else)
         *   - So it is of utmost importance that you invalidate all references you have to the given
         *     data pointer as ownership goes back to this PoolAllocator!
         *   - This method does not call a destructor. If you pass an object here, you have to call
         *     its destructor yourself before calling free().
         * Does not throw any exceptions at will so that you can call free in destructors.
         * Guarantee on absence of bugs is not given though so errors might still occur.
         * @param data The memory whose ownership should transferred back to this PoolAllocator.
         * @return True, iff freeing was successful.
         *         False, if the given memory belongs to this PoolAllocator but was not allocated.
         *         False, if the given memory was not allocated by this PoolAllocator.
         */
        PAX_NODISCARD bool free(void * data) noexcept override;

        /**
         * Checks whether the given data belongs to this PoolAllocator,
         * independent whether the data is actually allocated or not.
         * @param data The data which might or might not belong to this Allocator.
         * @return True, iff the given data is or can be allocated from this PoolAllocator.
         *         False, otherwise.
         */
        PAX_NODISCARD bool isMine(void * data) const override;

        /**
         * Clears the contents of this PoolAllocator and resets it,
         * such as if it would just have been created.
         * @return True iff clearing was successful.
         *         False iff there are still allocated elements that have to be freed first.
         */
        PAX_NODISCARD bool clear();

        /**
         * Returns metadata for the data at the given index.
         * @param index The index of which metadata should be given.
         * @return A pointer to the metadata about the element at the given index.
         */
        PAX_NODISCARD ChunkInfo * getChunkInfo(Index index) const;

        /**
         * Returns the data at the given index.
         * This data may be unused (not allocated) or allocated.
         * To find out, call getChunkInfo for the same index.
         * @param index The index whose data should be given.
         * @return A pointer to the data at the given index.
         *         The returned pointer will be of the size returned by 'getAllocationSize()'.
         */
        PAX_NODISCARD void * getData(Index index) const;

        /**
         *
         * @return The size of each allocated data object..
         *         This equals the value of the parameter elementSize
         *         that was passed to this PoolAllocator during creation.
         */
        PAX_NODISCARD size_t getAllocationSize() const override;

        /**
         * @return The maximum number of elements that can be allocated simultaneously.
         */
        PAX_NODISCARD Index getCapacity() const;

        /**
         * Points to the first allocated element in this PoolAllocator.
         * @return Index of the first allocated element.
         */
        PAX_NODISCARD Index begin() const;

        /**
         * Points one behind the last allocated element in this PoolAllocator.
         * So there is no valid element at end().
         * The last valid element is at (end() - 1).
         * @return Index one after the last allocated element.
         */
        PAX_NODISCARD Index end() const;

        /**
         * Sets the default capacity of new PoolAllocators to the given value.
         * The default capacity is used when constructing new PoolAllocators without
         * specifying a capacity explicitly.
         * A pool allocator's capacity denotes the maximum number of elements it can contain.
         * Does not affect existing pool allocators.
         * @param defaultCapacity
         */
        PAX_MAYBEUNUSED static void SetDefaultCapacity(size_t defaultCapacity);
        PAX_MAYBEUNUSED PAX_NODISCARD static size_t GetDefaultCapacity();
    };
}

#endif //POLYPROPYLENE_POOLALLOCATOR_H
