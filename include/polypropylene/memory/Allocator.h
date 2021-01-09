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
        virtual void free(void * data) = 0;

        /**
         * @return The size of each allocated data object.
         */
        virtual size_t getAllocationSize() = 0;
    };
}

#endif //POLYPROPYLENE_PROPERTYALLOCATOR_H
