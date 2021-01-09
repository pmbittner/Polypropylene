//
// Created by Bittner on 18/01/2019.
//

#ifndef POLYPROPYLENE_DEFINITIONS_H
#define POLYPROPYLENE_DEFINITIONS_H

#include "CXXVersionDetection.h"

#define PAX_NON_CONST
#define PAX_IMPLICIT

#ifdef UNREFERENCED_PARAMETER
#define PAX_UNREFERENCED_PARAMETER(P) UNREFERENCED_PARAMETER(P);
#else
#define PAX_UNREFERENCED_PARAMETER(P) (P);
#endif

#if PAX_CXX_STANDARD >= 2017
    #define PAX_CONSTEXPR_IF if constexpr
    #define PAX_NODISCARD [[nodiscard]]
    #define PAX_MAYBEUNUSED [[maybe_unused]]
#else
    #define PAX_CONSTEXPR_IF if
    #define PAX_NODISCARD
    #define PAX_MAYBEUNUSED
#endif

#define PAX_INTERNAL(name) _paxinternal_##name

#define PAX_STRINGIFY_2_(a) #a
#define PAX_STRINGIFY(a) PAX_STRINGIFY_2_(a)

#endif //POLYPROPYLENE_DEFINITIONS_H
