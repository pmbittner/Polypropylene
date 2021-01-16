//
// Created by Paul Bittner on 16.01.2021.
//

#ifndef POLYPROPYLENE_ASSERT_H
#define POLYPROPYLENE_ASSERT_H

#include "Errors.h"

#define PAX_ASSERT(...) do { \
    if (!(__VA_ARGS__)) {    \
        PAX_THROW_RUNTIME_ERROR("Assertion (" << PAX_STRINGIFY(__VA_ARGS__) << ") failed!"); \
    }                        \
} while(0)

#if PAX_BUILD_TYPE_DEBUG
    #define PAX_DEBUGASSERT(...) PAX_ASSERT(__VA_ARGS__)
#else
    // As the arguments to the assertion might be side-effectual, still use them.
    #define PAX_DEBUGASSERT(...) (__VA_ARGS__)
#endif

#endif //POLYPROPYLENE_ASSERT_H
