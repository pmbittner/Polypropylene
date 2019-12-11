//
// Created by Bittner on 08.08.2019.
//

#ifndef PAXENGINE3_TRYPARSER_H
#define PAXENGINE3_TRYPARSER_H

#include "macros/Definitions.h"

namespace PAX {
    template<class From, class To>
    class TryParser {
    public:
        PAX_NODISCARD static To tryParse(const From & f) {
            return f;
        }
    };
}

#endif //PAXENGINE3_TRYPARSER_H
