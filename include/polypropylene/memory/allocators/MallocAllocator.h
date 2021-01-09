//
// Created by paul on 22.09.18.
//

#ifndef POLYPROPYLENE_PROPERTYMALLOCALLOCATOR_H
#define POLYPROPYLENE_PROPERTYMALLOCALLOCATOR_H

#include "../Allocator.h"
#include <cstdlib>

namespace PAX {
    /**
     * Allocator that simply mallocs or frees single elements.
     */
    class MallocAllocator : public IAllocator {
        const size_t elementSize;

    public:
        MallocAllocator(size_t elementSize);

        void* allocate() override;
        void free(void * data) override;
        size_t getAllocationSize() override;
    };
}

#endif //POLYPROPYLENE_PROPERTYMALLOCALLOCATOR_H
