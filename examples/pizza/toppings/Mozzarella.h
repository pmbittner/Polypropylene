//
// Created by Bittner on 06.12.2019.
//

#ifndef POLYPROPYLENE_GOUDA_H
#define POLYPROPYLENE_GOUDA_H

#include "Cheese.h"

namespace PAX::Examples {
    class Mozzarella : public Cheese {
        PAX_PROPERTY(Mozzarella, PAX_PROPERTY_IS_CONCRETE)
        PAX_PROPERTY_DERIVES(Cheese)
        PAX_PROPERTY_IS_SINGLE

    public:
        std::string yummy() override;
    };
}

#endif //POLYPROPYLENE_GOUDA_H
