//
// Created by Paul Bittner on 10.01.2021.
//

#ifndef POLYPROPYLENE_PAXTEST_H
#define POLYPROPYLENE_PAXTEST_H

#include "gtest/gtest.h"
#include "polypropylene/definitions/Definitions.h"

#include <unordered_set>

#define PAX_TEST(testsuite, testcase) \
    TEST(testsuite, testcase) {       \
        std::cout << "Running test [" << PAX_STRINGIFY(testsuite) << ": " << PAX_STRINGIFY(testcase) << "]";

namespace PAX {
    template<typename T>
    bool ContentEquals(const std::vector<T> & a, const std::vector<T> & b) {
        return std::unordered_set<T>(a.begin(), a.end()) == std::unordered_set<T>(b.begin(), b.end());
    }
}

#endif //POLYPROPYLENE_PAXTEST_H
