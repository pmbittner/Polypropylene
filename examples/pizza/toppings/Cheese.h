//
// Created by Bittner on 05.12.2019.
//

#ifndef POLYPROPYLENE_CHEESE_H
#define POLYPROPYLENE_CHEESE_H

#include "../Topping.h"
#include "TomatoSauce.h"
#include "../BakedEvent.h"

namespace PAX::Examples {
    class Cheese : public Topping {
        PAX_PROPERTY(Cheese, PAX_PROPERTY_IS_ABSTRACT)
        PAX_PROPERTY_DERIVES(Topping)
        PAX_PROPERTY_IS_MULTIPLE

        PAX_PROPERTY_DEPENDS_ON(TomatoSauce)

    public:
        std::string yummy() override;

        void attached(Pizza & pizza) override;
        void detached(Pizza & pizza) override;

        void baked(BakedEvent & e);
    };
}

#endif //POLYPROPYLENE_CHEESE_H
