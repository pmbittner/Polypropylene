//
// Created by paul on 22.09.18.
//

#ifndef POLYPROPYLENE_PROPERTYALLOCATOR_H
#define POLYPROPYLENE_PROPERTYALLOCATOR_H

namespace PAX {
    class IAllocator {
    public:
        virtual ~IAllocator() = default;
        virtual void* allocate() = 0;
        virtual void destroy(void * data) = 0;
        virtual size_t getAllocationSize() = 0;
    };

    template<size_t ElementSize>
    class Allocator : public IAllocator {
    public:
        virtual size_t getAllocationSize() {
            return ElementSize;
        }
    };
}

#endif //POLYPROPYLENE_PROPERTYALLOCATOR_H
