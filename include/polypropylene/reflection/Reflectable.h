//
// Created by Bittner on 01.01.2020.
//

#ifndef POLYPROPYLENE_REFLECTABLE_H
#define POLYPROPYLENE_REFLECTABLE_H

#include "ClassMetadata.h"

namespace PAX {
    class Reflectable {
    public:
        /**
         * @return Reflection information on the fields of this class.
         */
        PAX_NODISCARD virtual ClassMetadata getMetadata() = 0;
    };
}

#endif //POLYPROPYLENE_REFLECTABLE_H
