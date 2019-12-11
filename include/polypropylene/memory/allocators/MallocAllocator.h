//
// Created by paul on 22.09.18.
//

#ifndef POLYPROPYLENE_PROPERTYMALLOCALLOCATOR_H
#define POLYPROPYLENE_PROPERTYMALLOCALLOCATOR_H

#include "../Allocator.h"
#include <cstdlib>

namespace PAX {
    template<size_t ElementSize>
    class MallocAllocator : public Allocator<ElementSize> {
    public:
        void* allocate() override {
            return malloc(ElementSize);
        }

        void destroy(void * data) override {
            free(data);
        }
    };
}

#endif //POLYPROPYLENE_PROPERTYMALLOCALLOCATOR_H
