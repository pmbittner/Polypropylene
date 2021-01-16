//
// Created by paul on 22.09.18.
//

#ifndef POLYPROPYLENE_PROPERTYMALLOCALLOCATOR_H
#define POLYPROPYLENE_PROPERTYMALLOCALLOCATOR_H

#include "../Allocator.h"
#include <unordered_set>

namespace PAX {
    /**
     * Allocator that simply uses malloc and free.
     */
    class MallocAllocator : public IAllocator {
        const size_t elementSize;
        std::unordered_set<void*> allocatedObjects;

    public:
        explicit MallocAllocator(size_t elementSize);

        PAX_NODISCARD void* allocate() override;
        PAX_NODISCARD bool free(void * data) override;
        PAX_NODISCARD size_t getAllocationSize() const override;
        PAX_NODISCARD bool isMine(void * data) const override;
    };
}

#endif //POLYPROPYLENE_PROPERTYMALLOCALLOCATOR_H
