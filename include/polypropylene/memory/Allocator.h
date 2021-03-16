//
// Created by paul on 22.09.18.
//

#ifndef POLYPROPYLENE_PROPERTYALLOCATOR_H
#define POLYPROPYLENE_PROPERTYALLOCATOR_H

//#include <cstddef> // for size_t
#include <string>
#include "polypropylene/definitions/Definitions.h"

namespace PAX {
    /**
     * Interface for memory allocators.
     */
    class Allocator {
        std::string name;

    public:
        explicit Allocator(const std::string & name);

        virtual ~Allocator();

        /**
         * Allocates a memory chunk of size @see getAllocationSize().
         * @return A memory chunk of size @see getAllocationSize().
         * @see getAllocationSize
         */
        PAX_NODISCARD virtual void* allocate() = 0;

        /**
         * Frees a memory chunk that was allocated with this allocator.
         * @param data The data to free (i.e., deallocate).
         * @return True iff the given memory was successfully deallocated.
         *         May return false if the given data was not allocated
         *         by this allocator, was already freed, or there was a failure.
         */
        PAX_NODISCARD virtual bool free(void * data) = 0;

        /**
         * @return True iff the given memory was allocated by this allocator.
         */
        PAX_NODISCARD virtual bool isMine(void * data) const  = 0;

        /**
         * @return The size of each allocated data object.
         */
        PAX_NODISCARD virtual size_t getAllocationSize() const = 0;

        PAX_NODISCARD const std::string & getName() const;
    };
}

#endif //POLYPROPYLENE_PROPERTYALLOCATOR_H
