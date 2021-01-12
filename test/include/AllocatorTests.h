//
// Created by Paul Bittner on 10.01.2021.
//

#ifndef POLYPROPYLENE_ALLOCATORTESTS_H
#define POLYPROPYLENE_ALLOCATORTESTS_H

#include "PaxTest.h"

#include "toppings/TomatoSauce.h"
#include "polypropylene/memory/PropertyPool.h"

namespace PAX {
    static void expect_equal(
            const PropertyPool<Examples::TomatoSauce> & pool,
            std::vector<Examples::TomatoSauce*> sauces)
    {
        int i = 0;
        for (Examples::TomatoSauce * poolsSauce : pool) {
            EXPECT_EQ(sauces.at(i)->getScoville(), poolsSauce->getScoville());
            ++i;
        }
        EXPECT_EQ(i, sauces.size()) << "PropertyPool and vector have different sizes!";
    }

    PAX_TEST(Allocator, IteratingPropertyPoolYieldsExactlyAllAllocatedInstances)
        using namespace PAX::Examples;

        PropertyPool<TomatoSauce> pool;
        std::vector<TomatoSauce*> sauces;

        sauces.emplace_back(pax_new(TomatoSauce)(5));
        sauces.emplace_back(pax_new(TomatoSauce)(999));
        sauces.emplace_back(pax_new(TomatoSauce)(0));
        expect_equal(pool, sauces);

        ASSERT_TRUE(pax_delete(sauces.back()));
        sauces.pop_back();
        expect_equal(pool, sauces);

        sauces.emplace_back(pax_new(TomatoSauce)(700));
        expect_equal(pool, sauces);

        for (TomatoSauce * t : sauces) {
            ASSERT_TRUE(pax_delete(t));
        }
        sauces.clear();

        for (PAX_MAYBEUNUSED Examples::TomatoSauce * ts : sauces) {
            EXPECT_TRUE(false) << "The pool still contains instances of TomatoSauce although we deleted all of them!";
        }
    }
}

#endif //POLYPROPYLENE_ALLOCATORTESTS_H
