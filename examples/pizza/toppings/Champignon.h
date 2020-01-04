//
// Created by Bittner on 06.12.2019.
//

#ifndef POLYPROPYLENE_CHAMPIGNON_H
#define POLYPROPYLENE_CHAMPIGNON_H

#include "../Topping.h"

namespace PAX::Examples {
    class Champignon : public Topping {
        PAX_PROPERTY(Champignon, PAX_PROPERTY_IS_CONCRETE)
        PAX_PROPERTY_DERIVES(Topping)
        PAX_PROPERTY_IS_SINGLE

    public:
        std::string yummy() override;
    };
}

#endif //POLYPROPYLENE_CHAMPIGNON_H
