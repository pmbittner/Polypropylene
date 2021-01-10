//
// Created by Paul Bittner on 10.01.2021.
//

#ifndef POLYPROPYLENE_PAXTEST_H
#define POLYPROPYLENE_PAXTEST_H

#include "gtest/gtest.h"
#include "polypropylene/definitions/Definitions.h"

#define PAX_TEST(testsuite, testcase) \
    TEST(testsuite, testcase) {       \
        std::cout << "Running test [" << PAX_STRINGIFY(testsuite) << ": " << PAX_STRINGIFY(testcase) << "]";

#endif //POLYPROPYLENE_PAXTEST_H
