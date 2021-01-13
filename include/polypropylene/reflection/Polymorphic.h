//
// Created by Paul Bittner on 12.01.2021.
//

#ifndef POLYPROPYLENE_POLYMORPHIC_H
#define POLYPROPYLENE_POLYMORPHIC_H

#include "TypeHandle.h"

namespace PAX {
    class Polymorphic {
    public:
        /**
         * @return A TypeHandle identifying the actual type of the object (i.e., the derived class).
         */
        PAX_NODISCARD virtual const TypeHandle& getClassType() const = 0;
    };
}

#endif //POLYPROPYLENE_POLYMORPHIC_H
