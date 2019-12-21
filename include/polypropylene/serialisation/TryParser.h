//
// Created by Bittner on 08.08.2019.
//

#ifndef POLYPROPYLENE_TRYPARSER_H
#define POLYPROPYLENE_TRYPARSER_H

#include "polypropylene/definitions/Definitions.h"

namespace PAX {
    template<class From, class To>
    class TryParser {
    public:
        PAX_NODISCARD static To tryParse(const From & f) {
            return f;
        }
    };
}

#endif //POLYPROPYLENE_TRYPARSER_H
