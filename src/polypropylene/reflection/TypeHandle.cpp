//
// Created by paul on 22.12.19.
//
#include <cstddef>
#include "polypropylene/reflection/TypeHandle.h"

namespace PAX {
    TypeHandle::TypeHandle(const std::type_index & type, size_t size) : typeindex(type), size(size) {

    }

    const char * TypeHandle::name() const noexcept {
        return typeindex.name();
    }

    size_t TypeHandle::hash_code() const noexcept {
        return typeindex.hash_code();
    }

    bool TypeHandle::operator==(const TypeHandle & other) const noexcept {
        return this->typeindex == other.typeindex && this->size == other.size;
    }

    TypeHandle::operator std::type_index() const noexcept {
        return typeindex;
    }
}