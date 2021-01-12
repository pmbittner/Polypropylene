//
// Created by Paul Bittner on 12.01.2021.
//

#ifndef POLYPROPYLENE_ENTITYTESTS_H
#define POLYPROPYLENE_ENTITYTESTS_H

#include "PaxTest.h"

#include "Pizza.h"

namespace PAX {
    PAX_TEST(Entity, GettingAllProperties)
        using namespace Examples;
        Pizza * pizza = pax_new(Pizza)();

        std::vector<Topping*> toppings;
        toppings.push_back(pax_new(Champignon)());
        toppings.push_back(pax_new(TomatoSauce)(1));
        toppings.push_back(pax_new(Mozzarella)());

        // Add the Toppings to the pizza
        for (Topping * topping : toppings) {
            pizza->add(topping);
        }

        EXPECT_TRUE(ContentEquals(toppings, pizza->getAllProperties()));

        pax_delete(pizza);
    }
}

#endif //POLYPROPYLENE_ENTITYTESTS_H
