//
// Created by Bittner on 05.12.2019.
//

#ifndef PROTEUS_PIZZA_H
#define PROTEUS_PIZZA_H

#include <polypropylene/property/PropertyContainer.h>

namespace Proteus::Examples {
    class Pizza : public PAX::PropertyContainer<Pizza> {
    public:
        void yummy();
        void bake();
    };
}
#endif //PROTEUS_PIZZA_H
