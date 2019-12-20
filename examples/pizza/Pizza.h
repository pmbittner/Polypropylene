//
// Created by Bittner on 05.12.2019.
//

#ifndef PROTEUS_PIZZA_H
#define PROTEUS_PIZZA_H

#include <polypropylene/property/Entity.h>

namespace PAX::Examples {
    class Pizza : public PAX::Entity<Pizza> {
    public:
        void yummy();
        void bake();
    };
}

#endif //PROTEUS_PIZZA_H
