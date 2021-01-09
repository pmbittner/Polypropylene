//
// Created by Bittner on 14/08/2019.
//

#ifndef POLYPROPYLENE_POOLALLOCATOR_H
#define POLYPROPYLENE_POOLALLOCATOR_H

#include "../Allocator.h"
#include <polypropylene/log/Errors.h>
#include <cstring>

namespace PAX {
    /// A PoolAllocator of fixed capacity.
    /// It can neither grow nor shrink.
    class PoolAllocator : public IAllocator {
    public:
        static constexpr size_t DefaultSize = 1024;
        using Index = int32_t;

        struct ChunkInfo {
        public:
            bool allocated;
        };

    private:
        /// Chunk data
        using memunit = char;
        static constexpr size_t MetaDataSize = sizeof(ChunkInfo);

        /// This is a stack to remember which chunks of memory are free.
        /// The stack is realised as an array where the top is at the beginning and is moved to the back as elements get popped.
        /// Hence, when an element is pushed, topIndex is decremented.
        struct IndexStack {
            Index * stack;
            Index topIndex;

            IndexStack();
            IndexStack(IndexStack && other) noexcept;

            Index pop();
            void push(Index val);
        } freeChunks;

        /// Memory
        const Index capacity;
        const size_t elementSize;
        memunit * memory = nullptr;
        int32_t numberOfAllocations = 0;

        size_t ChunkSize() const;
        size_t MemorySize() const;
        
        void clearFreeChunksStack();
        bool areFreeChunksAvailable() const;

        static void * DataOf(ChunkInfo * chunk);
        static memunit * FromData(void * data);
        static ChunkInfo* InfoFor(memunit * chunk);
        Index indexOf(memunit * m) const;
        memunit* memAtIndex(Index index) const;

    public:
        /// Creates a PoolAllocator of fixedSize.
        /// \param capacity The maximum number of elements that can be allocated simultaneously.
        explicit PoolAllocator(size_t elementSize, Index capacity = DefaultSize);
        PoolAllocator(PAX::PoolAllocator && other) noexcept;
        PoolAllocator(const PoolAllocator & other) = delete;
        
        PoolAllocator & operator=(const PoolAllocator & other) = delete;

        ~PoolAllocator() override;

        void * allocate() override;
        void free(void * data) override;

        void clear();
        
        ChunkInfo * getChunkInfo(Index index) const;
        void * getData(Index index) const;
        
        size_t getAllocationSize() override;
        Index getCapacity() const;
    };
}

#endif //POLYPROPYLENE_POOLALLOCATOR_H
