//
// Created by Paul on 14.01.2018.
//

#ifndef POLYPROPYLENE_TYPEINFO_H
#define POLYPROPYLENE_TYPEINFO_H

#include <typeindex>
#include "polypropylene/definitions/Definitions.h"

namespace PAX {
    using TypeId = std::type_index;

    struct TypeHandle {
        TypeId id;
        size_t size;

        TypeHandle(const TypeId & type, size_t size);
        PAX_NODISCARD const char * name() const noexcept;
        PAX_NODISCARD size_t hash_code() const noexcept;
        bool operator==(const TypeHandle & other) const noexcept;
        explicit operator TypeId() const noexcept;
    };
}

#define paxtypeid(T) typeid(T)
#define paxtypeof(T) ::PAX::TypeHandle(paxtypeid(T), sizeof(T))

#endif //POLYPROPYLENE_TYPEINFO_H
