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
        static constexpr size_t Initial_Size = 1024;
        using Index = int32_t;

        struct ChunkInfo {
        public:
            bool allocated;
        };

    private:
        /// Chunk data
        using memunit = char;
        static constexpr size_t MetaDataSize = sizeof(ChunkInfo);

        size_t ChunkSize() const { return MetaDataSize + elementSize; }

        static void * DataOf(ChunkInfo * chunk) {
            return reinterpret_cast<memunit*>(chunk) + MetaDataSize;
        }

        static memunit * fromData(void * data) {
            // The given pointer points to the data inside a ChunkInfo.
            // DataOffset describes the byte offset of this data.
            // Using it allows us to jump to the start of the ChunkInfo.
            return reinterpret_cast<memunit*>(data) - MetaDataSize;
        }

        static ChunkInfo* infoFor(memunit * chunk) {
            return reinterpret_cast<ChunkInfo*>(chunk);
        }

        /// memory
        const Index capacity;
        const size_t elementSize;
        memunit * memory = nullptr;
        int32_t numberOfAllocations = 0;

        size_t MemorySize() const { return capacity * ChunkSize(); }

        Index indexOf(memunit * m) const {
            return Index((m - memory) / ChunkSize());
        }

        memunit* memAtIndex(Index index) const {
            return memory + size_t(index * ChunkSize());
        }

        /// This is a stack to remember which MemoryChunks are free.
        /// The stack is realised as an array where the top is at the beginning and is moved to the back as elements get popped.
        /// Hence, when an element is pushed, topIndex is decremented.
        struct {
            Index * stack;
            Index topIndex;

            Index pop() {
                Index ret = stack[topIndex];
                ++topIndex;
                return ret;
            }

            void push(Index val) {
                // Decrement the topIndex to make space for our new val.
                --topIndex;
                // Insert such that the stack is sorted and the smallest index stays at the stack top.
                // Possible future optimisation:
                // The stack needs not to be sorted. We only need the smallest element of the stack at the front.
                Index posToInsertFreeIndex = topIndex;
                while (stack[posToInsertFreeIndex + 1] < val) {
                    stack[posToInsertFreeIndex] = stack[posToInsertFreeIndex + 1];
                    ++posToInsertFreeIndex;
                }

                stack[posToInsertFreeIndex] = val;
            }
        } freeChunks;
        
        void clearStack() {
            // Initialise free chunks stack: All chunks are free now.
            freeChunks.topIndex = 0;
            for (Index i = 0; i < capacity; ++i) {
                freeChunks.stack[i] = i;
            }
        }
        
        bool freeChunksAvailable() const {
            return freeChunks.topIndex < capacity;
        }

    public:
        /// Creates a PoolAllocator of fixedSize.
        /// \param capacity The maximum number of elements that can be allocated simultaneously.
        explicit PoolAllocator(size_t elementSize, Index capacity = Initial_Size)
        : elementSize(elementSize), capacity(capacity), numberOfAllocations(0)
        {
            memory = new memunit[MemorySize()];
            freeChunks.stack = new Index[capacity];
            clear();
        }

        PoolAllocator(const PAX::PoolAllocator && other)
        : elementSize(other.elementSize),
          capacity(other.capacity),
          memory(other.memory)
        {
            freeChunks.stack = other.freeChunks.stack;
            clear();
        }

        PoolAllocator(const PoolAllocator & other) = delete;
        PoolAllocator & operator=(const PoolAllocator & other) = delete;

        ~PoolAllocator() override {
            if (numberOfAllocations > 0) {
                PAX_LOG(PAX::Log::Level::Warn, "Deleting PoolAllocator although there are still " << numberOfAllocations << " elements allocated!");
            }

            delete[] memory;
            delete[] freeChunks.stack;
        }

        void * allocate() override {
            if (freeChunksAvailable()) {
                ++numberOfAllocations;
                ChunkInfo * ourChunkInfo = getChunkInfo(freeChunks.pop());
                ourChunkInfo->allocated = true;
                return DataOf(ourChunkInfo);
            } else {
                PAX_THROW_RUNTIME_ERROR("Memory overflow");
            }
        };

        void destroy(void * data) override {
            memunit * mem = fromData(data);
            const Index i = indexOf(mem);

            if (0 <= i && i < capacity) {
                ChunkInfo * chunkToFree = infoFor(mem);
                if (chunkToFree->allocated) {
                    chunkToFree->allocated = false;
                    --numberOfAllocations;
                    freeChunks.push(i);
                } else {
                    PAX_LOG(PAX::Log::Level::Warn, "Trying to free unallocated memory chunk! aborting...");
                }
            } else {
                // The data seems not to be allocated by us.
                // We should not throw an exception as this function is likely to be called in destructors.
                PAX_LOG(PAX::Log::Level::Error, "Given pointer (" << data << ") was not allocated by me!");
            }
        };

        void clear() {
            if (numberOfAllocations == 0) {
                // Null memory. (Thereby set allocated to false in all chunks).
                memset(memory, 0, MemorySize());
                clearStack();
            } else {
                PAX_LOG(PAX::Log::Level::Error, "Clearing PoolAllocator although there are still " << numberOfAllocations << " elements allocated");
            }
        }

        size_t getAllocationSize() override {
            return elementSize;
        }

        ChunkInfo * getChunkInfo(Index index) const {
            return infoFor(memAtIndex(index));
        }

        void * getData(Index index) const {
            return DataOf(getChunkInfo(index));
        }

        Index getCapacity() {
            return capacity;
        }
    };
}

#endif //POLYPROPYLENE_POOLALLOCATOR_H
