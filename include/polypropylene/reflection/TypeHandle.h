//
// Created by Paul on 14.01.2018.
//

#ifndef POLYPROPYLENE_TYPEINFO_H
#define POLYPROPYLENE_TYPEINFO_H

#include <typeindex>
#include "polypropylene/definitions/Definitions.h"

namespace PAX {
    struct TypeHandle {
        std::type_index typeindex;
        size_t size;

        TypeHandle(const std::type_index & type, size_t size);
        PAX_NODISCARD const char * name() const noexcept;
        PAX_NODISCARD size_t hash_code() const noexcept;
        bool operator==(const TypeHandle & other) const noexcept;
        operator std::type_index() const noexcept;
    };
}

#define paxtypeof(T) ::PAX::TypeHandle(typeid(T), sizeof(T))

#endif //POLYPROPYLENE_TYPEINFO_H
