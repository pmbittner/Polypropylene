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
    class PoolAllocator : public IAllocator {
    public:
        static constexpr size_t DefaultCapacity = 1024;
        using Index = int32_t;

        /**
         * Metadata for each allocatable element.
         */
        struct ChunkInfo {
        public:
            bool allocated;
        };

    private:
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

        size_t ChunkSize() const;
        size_t MemorySize() const;

        /**
         * @return The user data that is associated to the given metadata.
         */
        static void * DataOf(ChunkInfo * chunk);

        /**
         * @return A pointer into 'memory' to the beginning of this data's chunk
         * (w.r.t. to its prefixing ChunkInfo).
         */
        static memunit * FromData(void * data);

        /**
         * Assumes that the given pointer points to the begin of a data chunk in 'memory'
         * (i.e., (chunk - memory) % ChunkSize() == 0).
         * @return The metadata for the data chunk at the given pointer into 'memory'.
         */
        ChunkInfo* InfoFor(memunit * chunk) const;

        /**
         * Assumes that the given pointer points to the begin of a data chunk in 'memory'
         * (i.e., (chunk - memory) % ChunkSize() == 0).
         * @return The index of the data chunk at the given pointer into 'memory'.
         */
        Index indexOf(const memunit * m) const;

        /**
         * @return A pointer to the data chunk inside 'memory' at the given index.
         */
        memunit* memAtIndex(Index index) const;

    public:
        /**
         * Creates a PoolAllocator of a fixed capacity.
         * @param elementSize The size each allocated data object should have.
         * @param capacity The maximum number of elements that can be allocated simultaneously.
         */
        explicit PoolAllocator(size_t elementSize, Index capacity = DefaultCapacity);
        PoolAllocator(PAX::PoolAllocator && other) noexcept;

        PoolAllocator(const PoolAllocator & other) = delete;
        PoolAllocator & operator=(const PoolAllocator & other) = delete;

        ~PoolAllocator() override;

        void * allocate() override;
        void free(void * data) override;

        /**
         * Clears the contents of this PoolAllocator and resets it,
         * such as if it would just have been created.
         */
        void clear();

        /**
         * Returns metadata for the data at the given index.
         * @param index The index of which metadata should be given.
         * @return A pointer to the metadata about the element at the given index.
         */
        ChunkInfo * getChunkInfo(Index index) const;

        /**
         * Returns the data at the given index.
         * This data may be unused (not allocated) or allocated.
         * To find out, call getChunkInfo for the same index.
         * @param index The index whose data should be given.
         * @return A pointer to the data at the given index.
         *         The returned pointer will be of the size returned by 'getAllocationSize()'.
         */
        void * getData(Index index) const;

        /**
         *
         * @return The size of each allocated data object..
         *         This equals the value of the parameter elementSize
         *         that was passed to this PoolAllocator during creation.
         */
        size_t getAllocationSize() override;

        /**
         * @return The maximum number of elements that can be allocated simultaneously.
         */
        Index getCapacity() const;
    };
}

#endif //POLYPROPYLENE_POOLALLOCATOR_H
