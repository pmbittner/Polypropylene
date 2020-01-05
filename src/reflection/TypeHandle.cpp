//
// Created by paul on 22.12.19.
//
#include <cstddef>
#include "polypropylene/reflection/TypeHandle.h"

namespace PAX {
    TypeHandle::TypeHandle(const TypeId & type, size_t size) : id(type), size(size) {

    }

    const char * TypeHandle::name() const noexcept {
        return id.name();
    }

    size_t TypeHandle::hash_code() const noexcept {
        return id.hash_code();
    }

    bool TypeHandle::operator==(const TypeHandle & other) const noexcept {
        return this->id == other.id && this->size == other.size;
    }

    TypeHandle::operator TypeId() const noexcept {
        return id;
    }
}