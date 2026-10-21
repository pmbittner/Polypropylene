//
// Created by Paul on 19.12.2017.
//

#ifndef POLYPROPYLENE_TEMPLATETYPETOSTRING_H
#define POLYPROPYLENE_TEMPLATETYPETOSTRING_H

#include <polypropylene/definitions/CompilerDetection.h>

#ifdef PAX_COMPILER_MSVC
#include <memory.h>
#else
#include <typeinfo>
#endif

namespace PAX {
    namespace Reflection {
#ifdef PAX_COMPILER_MSVC
        /**
         * This code is copied from https://blog.molecular-matters.com/2015/12/11/getting-the-type-of-a-template-argument-as-string-without-rtti/
         * at 2017 12 19
         *
         * Adapted to GCC
         *
         * Only used for MSVC: it relies on __FUNCTION__ being the qualified function name,
         * which is not the case for clang (there it is just "GetTypeName").
         */
        namespace internal {
            static const unsigned int FRONT_SIZE = sizeof("PAX::Reflection::internal::GetTypeNameHelper<") - 1u;
            static const unsigned int BACK_SIZE = sizeof(">::GetTypeName") - 1u;

            template<typename T>
            struct GetTypeNameHelper {
                static const char *GetTypeName() {
                    static const size_t size = sizeof(__FUNCTION__) - FRONT_SIZE - BACK_SIZE;
                    static char typeName[size] = {};
                    memcpy(typeName, __FUNCTION__ + FRONT_SIZE, size - 1u);
                    return typeName;
                }
            };
        }
#endif


        template<typename T>
        const char *GetTypeName() {
#ifdef PAX_COMPILER_MSVC
            return internal::GetTypeNameHelper<T>::GetTypeName();
#else
            return typeid(T).name();
#endif
        }
    }
}

#endif //POLYPROPYLENE_TEMPLATETYPETOSTRING_H
