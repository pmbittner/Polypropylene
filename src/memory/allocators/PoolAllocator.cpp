//
// Created by Paul Bittner on 09.01.2021.
//

#include <polypropylene/memory/allocators/PoolAllocator.h>

namespace PAX {
#ifdef PAX_BUILD_TYPE_DEBUG
    #define PAX_POOL_ASSERTVALIDINDEX(i) \
        if ((i) < 0 || capacity <= (i)) { \
            PAX_THROW_RUNTIME_ERROR("Index out of bounds. Can be in [0, " << getCapacity() << "] but was " << (i) << "!"); \
        }
#else
    #define PAX_POOL_ASSERTVALIDINDEX(i)
#endif

    size_t PoolAllocator::ChunkSize() const {
        return MetaDataSize + elementSize;
    }

    size_t PoolAllocator::MemorySize() const {
        return capacity * ChunkSize();
    }

    void * PoolAllocator::DataOf(ChunkInfo *chunk) {
        return reinterpret_cast<memunit*>(chunk) + MetaDataSize;
    }

    PoolAllocator::memunit * PoolAllocator::FromData(void *data) {
        // The given pointer points to the data inside a ChunkInfo.
        // DataOffset describes the byte offset of this data.
        // Using it allows us to jump to the start of the ChunkInfo.
        return reinterpret_cast<memunit*>(data) - MetaDataSize;
    }

    PoolAllocator::ChunkInfo * PoolAllocator::InfoFor(memunit *chunk) {
        return reinterpret_cast<ChunkInfo*>(chunk);
    }

    PoolAllocator::Index PoolAllocator::indexOf(const memunit * m) const {
        return Index((m - memory) / ChunkSize());
    }

    PoolAllocator::memunit * PoolAllocator::memAtIndex(Index index) const {
        PAX_POOL_ASSERTVALIDINDEX(index)
        return memory + size_t(index * ChunkSize());
    }

    PoolAllocator::PoolAllocator(size_t elementSize, Index capacity) :
      elementSize(elementSize),
      capacity(capacity),
      numberOfAllocations(0),
      freeChunks(capacity)
    {
        memory = new memunit[MemorySize()];
        clear();
    }

    PoolAllocator::PoolAllocator(PAX::PoolAllocator && other) noexcept :
      elementSize(other.elementSize),
      capacity(other.capacity),
      numberOfAllocations(other.numberOfAllocations),
      memory(other.memory),
      freeChunks(std::move(other.freeChunks))
    {
    }

    PoolAllocator::~PoolAllocator() {
        if (numberOfAllocations > 0) {
            PAX_LOG(PAX::Log::Level::Warn, "Deleting PoolAllocator although there are still " << numberOfAllocations << " elements allocated!");
        }

        delete[] memory;
    }

    PoolAllocator::IndexStack::IndexStack(Index capacity) :
      capacity(capacity),
      topIndex(0)
    {
        stack = new Index[capacity];
    }

    PoolAllocator::IndexStack::IndexStack(IndexStack && other) noexcept :
      capacity(other.capacity),
      topIndex(other.topIndex),
      stack(other.stack)
    {

    }

    PoolAllocator::IndexStack::~IndexStack() {
        delete[] stack;
    }

    PoolAllocator::Index PoolAllocator::IndexStack::pop() {
        Index ret = stack[topIndex];
        ++topIndex;
        return ret;
    }

    void PoolAllocator::IndexStack::push(Index val) {
        if (full()) {
            PAX_THROW_RUNTIME_ERROR("Memory overflow");
        }

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

    bool PoolAllocator::IndexStack::empty() const {
        return topIndex >= capacity;
    }

    bool PoolAllocator::IndexStack::full() const {
        return topIndex <= 0;
    }

    void PoolAllocator::clearFreeChunksStack() {
        // Initialise free chunks stack: All chunks are free now.
        freeChunks.topIndex = 0;
        for (Index i = 0; i < capacity; ++i) {
            freeChunks.stack[i] = i;
        }
    }

    void * PoolAllocator::allocate() {
        if (!freeChunks.empty()) {
            ++numberOfAllocations;
            ChunkInfo * ourChunkInfo = getChunkInfo(freeChunks.pop());
            ourChunkInfo->allocated = true;
            return DataOf(ourChunkInfo);
        } else {
            PAX_THROW_RUNTIME_ERROR("Memory overflow");
        }
    }

    void PoolAllocator::free(void *data) {
        memunit * mem = FromData(data);
        const Index i = indexOf(mem);

        if (0 <= i && i < capacity) {
            ChunkInfo * chunkToFree = InfoFor(mem);
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
    }

    void PoolAllocator::clear() {
        if (numberOfAllocations == 0) {
            // Null memory. (Thereby set allocated to false in all chunks).
            memset(memory, 0, MemorySize());
            clearFreeChunksStack();
        } else {
            PAX_LOG(PAX::Log::Level::Error, "Clearing PoolAllocator although there are still " << numberOfAllocations << " elements allocated");
        }
    }

    size_t PoolAllocator::getAllocationSize() {
        return elementSize;
    }

    PoolAllocator::ChunkInfo * PoolAllocator::getChunkInfo(Index index) const {
        PAX_POOL_ASSERTVALIDINDEX(index)
        return InfoFor(memAtIndex(index));
    }

    void * PoolAllocator::getData(Index index) const {
        PAX_POOL_ASSERTVALIDINDEX(index)
        return DataOf(getChunkInfo(index));
    }

    PoolAllocator::Index PoolAllocator::getCapacity() const {
        return capacity;
    }

#undef PAX_POOL_ASSERTVALIDINDEX
}