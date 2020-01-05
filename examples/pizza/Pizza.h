//
// Created by Bittner on 05.12.2019.
//

#ifndef POLYPROPYLENE_PIZZA_H
#define POLYPROPYLENE_PIZZA_H

#include <polypropylene/property/Entity.h>
#include "Topping.h"

namespace PAX::Examples {
    class Pizza : public PAX::Entity<Pizza, Topping> {
    public:
        void yummy();
        void bake();
    };
}

#endif //POLYPROPYLENE_PIZZA_H
