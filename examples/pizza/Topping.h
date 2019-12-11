//
// Created by Bittner on 05.12.2019.
//

#ifndef PROTEUS_TOPPING_H
#define PROTEUS_TOPPING_H

#include "Pizza.h"

namespace Proteus::Examples {
    class Topping : public PAX::Property<Pizza> {
        PAX_PROPERTY(Topping, PAX_PROPERTY_IS_ABSTRACT)
        PAX_PROPERTY_DERIVES(::PAX::Property<Pizza>)
        PAX_PROPERTY_IS_MULTIPLE

    public:
        virtual std::string yummy() = 0;
    };
}

#endif //PROTEUS_TOPPING_H
