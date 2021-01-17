//
// Created by Paul on 14.01.2018.
//

#ifndef POLYPROPYLENE_TYPEINFO_H
#define POLYPROPYLENE_TYPEINFO_H

#include <typeindex>
#include <cstddef> // size_t
#include "polypropylene/definitions/Definitions.h"

namespace PAX {
    using TypeId = std::type_index;

    struct Type {
        TypeId id;
        size_t size;

        Type(const TypeId & type, size_t size);
        PAX_NODISCARD const char * name() const noexcept;
        PAX_NODISCARD size_t hash_code() const noexcept;
        bool operator==(const Type & other) const noexcept;
        explicit operator TypeId() const noexcept;
    };

    /**
     * Models Single-Inheritance polymorphic types.
     */
    struct PolymorphicType {
        const Type type;
        const PolymorphicType * parent;

        PolymorphicType(const Type & type);
        PolymorphicType(const Type & type, const PolymorphicType * parent);
        PAX_NODISCARD bool hasParent() const noexcept;
        PAX_NODISCARD const char * name() const noexcept;
        PAX_NODISCARD size_t hash_code() const noexcept;
        bool operator==(const PolymorphicType & other) const noexcept;
    };
}

#define paxtypeid(...) typeid(__VA_ARGS__)
#define paxtypeof(...) ::PAX::Type(paxtypeid(__VA_ARGS__), sizeof(__VA_ARGS__))

#endif //POLYPROPYLENE_TYPEINFO_H
