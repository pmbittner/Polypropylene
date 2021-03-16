//
// Created by Paul Bittner on 09.01.2021.
//

#include "polypropylene/memory/allocators/PoolAllocator.h"
#include "polypropylene/log/Assert.h"

namespace PAX {
#ifdef PAX_BUILD_TYPE_DEBUG
    #define PAX_POOL_ASSERTVALIDINDEX(i) \
        if ((i) < 0 || capacity <= (i)) { \
            PAX_THROW_RUNTIME_ERROR("Index out of bounds in PoolAllocator " << getName() << ". Can be in [0, " << getCapacity() << "] but was " << (i) << "!"); \
        }
    #define PAX_POOL_ASSERTVALIDPOINTER(p) \
        if (((p) - memory) % ChunkSize() != 0) { \
            PAX_THROW_RUNTIME_ERROR("Pointer out of bounds in PoolAllocator " << getName() << ".Given pointer " << (p) << " does not point to the beginning a valid data chunk!"); \
        }
#else
    #define PAX_POOL_ASSERTVALIDINDEX(i)
    #define PAX_POOL_ASSERTVALIDPOINTER(p)
#endif

    size_t PoolAllocator::DefaultCapacity = 1024;

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

    PoolAllocator::ChunkInfo * PoolAllocator::InfoFor(memunit *chunk) const {
        PAX_POOL_ASSERTVALIDPOINTER(chunk)
        return reinterpret_cast<ChunkInfo*>(chunk);
    }

    PoolAllocator::Index PoolAllocator::indexOf(const memunit * m) const {
        PAX_POOL_ASSERTVALIDPOINTER(m)
        return Index((m - memory) / ChunkSize());
    }

    PoolAllocator::memunit * PoolAllocator::memAtIndex(Index index) const {
        PAX_POOL_ASSERTVALIDINDEX(index)
        return memory + size_t(index * ChunkSize());
    }

    void PoolAllocator::clearBounds() {
        firstElement = 0;
        lastElement = -1;
    }

    void PoolAllocator::updateBoundsAfterDeletionOf(Index i) {
        const bool isLastElement = i == lastElement;
        if (i == firstElement) {
            if (isLastElement) { // there was only one element allocated
                // and now we are empty
                clearBounds();
            } else {
                // go to the right and have a look who is the next allocated chunk
                for (Index whoIsIt = i + 1; whoIsIt <= lastElement; ++whoIsIt) {
                    if (getChunkInfo(whoIsIt)->allocated) {
                        firstElement = whoIsIt;
                        break;
                    }
                }
                // If we did not find any other allocated chunk
                // but we weren't the last element (because we are in the else branch)
                if (i == firstElement) {
                    // we had an illegal state
                    PAX_THROW_RUNTIME_ERROR("Illegal state in PoolAllocator " << getName() << ". This is a bug. The bounds begin and end are invalid.");
                }
            }
        } else if (isLastElement) {
            // go from the last position to the left until we hit the next allocated chunk
            for (Index whoIsIt = i - 1; whoIsIt >= firstElement; --whoIsIt) {
                if (getChunkInfo(whoIsIt)->allocated) {
                    lastElement = whoIsIt;
                    break;
                }
            }
            // If we did not find any other allocated chunk
            // but we weren't the first element (because we are in the else branch)
            if (i == lastElement) {
                // we had an illegal state
                PAX_THROW_RUNTIME_ERROR("Illegal state in PoolAllocator " << getName() << ". This is a bug. The bounds begin and end are invalid.");
            }
        }
    }

    PoolAllocator::PoolAllocator(const std::string & name, size_t elementSize, Index capacity) :
      Allocator(name),
      elementSize(elementSize),
      capacity(capacity),
      numberOfAllocations(0),
      freeChunks(capacity)
    {
        memory = new memunit[MemorySize()];
        PAX_ASSERT(clear());
    }

    PoolAllocator::PoolAllocator(PAX::PoolAllocator && other) noexcept :
      Allocator(other.getName()),
      elementSize(other.elementSize),
      capacity(other.capacity),
      numberOfAllocations(other.numberOfAllocations),
      memory(other.memory),
      firstElement(other.firstElement),
      lastElement(other.lastElement),
      freeChunks(std::move(other.freeChunks))
    {
    }

    PoolAllocator::~PoolAllocator() {
        if (numberOfAllocations > 0) {
            PAX_LOG(PAX::Log::Level::Warn, "Deleting PoolAllocator " << getName() << " although there are still " << numberOfAllocations << " elements allocated!");
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

    void PoolAllocator::IndexStack::clear() {
        // Initialise free chunks stack: All chunks are free now.
        topIndex = 0;
        for (Index i = 0; i < capacity; ++i) {
            stack[i] = i;
        }
    }

    bool PoolAllocator::IndexStack::empty() const {
        return topIndex >= capacity;
    }

    bool PoolAllocator::IndexStack::full() const {
        return topIndex <= 0;
    }

    void * PoolAllocator::allocate() {
        if (!freeChunks.empty()) {
            ++numberOfAllocations;
            Index indexOfNewElement = freeChunks.pop();
            ChunkInfo * ourChunkInfo = getChunkInfo(indexOfNewElement);
            ourChunkInfo->allocated = true;
            if (indexOfNewElement < firstElement) {
                firstElement = indexOfNewElement;
            }
            if (indexOfNewElement > lastElement) {
                lastElement = indexOfNewElement;
            }
            return DataOf(ourChunkInfo);
        } else {
            PAX_THROW_RUNTIME_ERROR("Memory overflow in PoolAllocator " << getName() << "!");
        }
    }

    bool PoolAllocator::free(void *data) noexcept {
        memunit * mem = FromData(data);
        const Index i = indexOf(mem);

        if (0 <= i && i < capacity) {
            ChunkInfo * chunkToFree = InfoFor(mem);
            if (chunkToFree->allocated) {
                chunkToFree->allocated = false;
                --numberOfAllocations;
                updateBoundsAfterDeletionOf(i);
                freeChunks.push(i);
                return true;
            } else {
                PAX_LOG(PAX::Log::Level::Warn, "Trying to free unallocated memory chunk in PoolAllocator " << getName() << "! aborting...");
            }
        } else {
            // The data seems not to be allocated by us.
            // We should not throw an exception as this function is likely to be called in destructors.
            PAX_LOG(PAX::Log::Level::Error, "Given pointer (" << data << ") was not allocated by me!");
        }

        return false;
    }

    bool PoolAllocator::isMine(void *data) const {
        // +1 to point to the end of the last data chunk = the begin of the first chunk behind our memory.
        void * end = static_cast<void*>(memory + MemorySize() + 1);
        return memory <= data && data < end;
    }

    bool PoolAllocator::clear() {
        if (numberOfAllocations == 0) {
            // Null memory. (Thereby set allocated to false in all chunks).
            memset(memory, 0, MemorySize());
            freeChunks.clear();
            clearBounds();
            return true;
        } else {
            PAX_LOG(PAX::Log::Level::Error, "Clearing PoolAllocator " << getName() << " although there are still " << numberOfAllocations << " elements allocated");
        }
        return false;
    }

    size_t PoolAllocator::getAllocationSize() const {
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

    PoolAllocator::Index PoolAllocator::begin() const {
        return firstElement;
    }

    PoolAllocator::Index PoolAllocator::end() const {
        // by C++ standard convention, end is supposed to point one behind the last valid element
        return lastElement + 1;
    }

    void PoolAllocator::SetDefaultCapacity(size_t defaultCapacity) {
        PAX_LOG(Log::Level::Info, "Changing default capacity from "
                << DefaultCapacity
                << " to "
                << defaultCapacity
                << ".");
        DefaultCapacity = defaultCapacity;
    }

    size_t PoolAllocator::GetDefaultCapacity() {
        return DefaultCapacity;
    }

#undef PAX_POOL_ASSERTVALIDINDEX
#undef PAX_POOL_ASSERTVALIDPOINTER
}