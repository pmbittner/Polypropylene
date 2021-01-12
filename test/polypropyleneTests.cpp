//
// Created by Paul Bittner on 10.01.2021.
//

#include "gtest/gtest.h"

#include "toppings/Mozzarella.h"
#include "toppings/Champignon.h"

/// These includes may be reported as unused but they are necessary!
/// If we do not include our tests here, RUN_ALL_TESTS won't find them.
#include "LogTests.h"
#include "AllocatorTests.h"
#include "EntityTests.h"

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);

    PAX_PROPERTY_REGISTER(PAX::Examples::Mozzarella);
    PAX_PROPERTY_REGISTER(PAX::Examples::Champignon);
    PAX_PROPERTY_REGISTER(PAX::Examples::TomatoSauce);

    return RUN_ALL_TESTS();
}