//
// Created by Bittner on 05.12.2019.
//

#ifndef POLYPROPYLENE_TOPPING_H
#define POLYPROPYLENE_TOPPING_H

#include <polypropylene/property/Property.h>
#include "Pizza.h"

namespace PAX::Examples {
    class Topping : public PAX::Property<Pizza> {
        PAX_PROPERTY(Topping, PAX_PROPERTY_IS_ABSTRACT)
        PAX_PROPERTY_DERIVES(::PAX::Property<Pizza>)
        PAX_PROPERTY_IS_MULTIPLE

    public:
        virtual std::string yummy() = 0;
    };
}

#endif //POLYPROPYLENE_TOPPING_H
