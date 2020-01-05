//
// Created by Bittner on 05.12.2019.
//

#ifndef POLYPROPYLENE_TOPPING_H
#define POLYPROPYLENE_TOPPING_H

#include <polypropylene/property/Property.h>

namespace PAX::Examples {
    class Pizza;

    class Topping : public PAX::Property<Pizza> {
        PAX_ROOT_PROPERTY(Topping, Pizza, PAX_PROPERTY_IS_ABSTRACT)

    public:
        virtual std::string yummy() = 0;
    };
}

#include "Pizza.h"

#endif //POLYPROPYLENE_TOPPING_H
