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
            EXPECT_TRUE(pizza->add(topping)) << "Adding Topping does not work";
        }

        EXPECT_TRUE(ContentEquals(toppings, pizza->getAllProperties()));

        EXPECT_TRUE(pax_delete(pizza));
    }

    PAX_TEST(Entity, Has)
        using namespace Examples;
        Pizza * pizza = pax_new(Pizza)();

        EXPECT_TRUE(pizza->add(pax_new(Champignon)()));
        EXPECT_TRUE(pizza->has<Topping>());
        EXPECT_TRUE(pizza->has<Champignon>());

        Mozzarella * mozzarella = pax_new(Mozzarella)();
        PAX_LOG_DEBUG(Log::Level::Info, "The following error message is expected");
        EXPECT_FALSE(pizza->add(mozzarella))
            << "Could add Mozzarella although dependency to tomato sauce is not met!";
        EXPECT_TRUE(pizza->add(pax_new(TomatoSauce)(1)));
        EXPECT_TRUE(pizza->add(mozzarella));

        EXPECT_TRUE(pizza->has<Topping>());
        EXPECT_TRUE(pizza->has<Champignon>());
        EXPECT_TRUE(pizza->has<Cheese>());
        EXPECT_TRUE(pizza->has<Mozzarella>());

        bool test = pizza->has<Mozzarella, Champignon>();
        EXPECT_TRUE(test);
        test = pizza->has<Champignon, TomatoSauce, Mozzarella>();
        EXPECT_TRUE(test);

        EXPECT_TRUE(pax_delete(pizza));
    }
}

#endif //POLYPROPYLENE_ENTITYTESTS_H
