//
// Created by paul on 22.09.18.
//

#ifndef POLYPROPYLENE_PROPERTYALLOCATOR_H
#define POLYPROPYLENE_PROPERTYALLOCATOR_H

namespace PAX {
    class IAllocator {
    public:
        virtual ~IAllocator() = default;
        virtual void destroy(void * data) = 0;
    };

    template<size_t ElementSize>
    class Allocator : public IAllocator {
    public:
        virtual void* allocate() = 0;
    };
}

#endif //POLYPROPYLENE_PROPERTYALLOCATOR_H
