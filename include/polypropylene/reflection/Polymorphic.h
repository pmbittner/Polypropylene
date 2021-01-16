//
// Created by Paul Bittner on 12.01.2021.
//

#ifndef POLYPROPYLENE_POLYMORPHIC_H
#define POLYPROPYLENE_POLYMORPHIC_H

#include "Type.h"

namespace PAX {
    class Polymorphic {
    public:
        /**
         * @return A TypeHandle identifying the actual type of the object (i.e., the derived class).
         */
        PAX_NODISCARD virtual const PolymorphicType & getClassType() const = 0;
    };
}

#endif //POLYPROPYLENE_POLYMORPHIC_H
