//
// Created by Paul Bittner on 27.12.2020.
//

#ifndef POLYPROPYLENE_CXXVERSIONDETECTION_H
#define POLYPROPYLENE_CXXVERSIONDETECTION_H

#include "CompilerDetection.h"

//#ifdef __cplusplus
//    #if __cplusplus <= 199711L
//        #define PAX_CXX_STANDARD 1997
//    #elif __cplusplus <= 201103L
//        #define PAX_CXX_STANDARD 2011
//    #elif __cplusplus <= 201402L
//        #define PAX_CXX_STANDARD 2014
//    #elif __cplusplus <= 201703L
//        #define PAX_CXX_STANDARD 2017
//    #else
//        #define PAX_CXX_STANDARD 2020
//    #endif
//#else
//    #ifdef PAX_COMPILER_MSVC
//        #pragma message ("[WARNING] Could not determine C++ version. Please set the value of the PAX_CXX_STANDARD macro manually (e.g., #define PAX_CXX_STANDARD 2017). The value should be the year number of the version.")
//    #else
//        #warning ("[WARNING] Could not determine C++ version. Please set the value of the PAX_CXX_STANDARD macro manually (e.g., #define PAX_CXX_STANDARD 2017). The value should be the year number of the version.")
//    #endif
//#endif

/**
 * We fix this to C++17 because an automated detection is troublesome with MSVC.
 * To use the __cplusplus macro, we need to add a compiler flag
 *     /Zc:__cplusplus
 * when compiling with MSVC.
 * As this has to be done manually be the user when using Polypropylene
 * and because we require at least C++17 either way, we hardcode this value.
 */
#define PAX_CXX_STANDARD 2017

#endif //POLYPROPYLENE_CXXVERSIONDETECTION_H
