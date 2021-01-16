//
// Created by paul on 22.12.19.
//
#include <cstddef>
#include "polypropylene/reflection/Type.h"

namespace PAX {
    Type::Type(const TypeId & type, size_t size) : id(type), size(size) {

    }

    const char * Type::name() const noexcept {
        return id.name();
    }

    size_t Type::hash_code() const noexcept {
        return id.hash_code();
    }

    bool Type::operator==(const Type & other) const noexcept {
        return this->id == other.id && this->size == other.size;
    }

    Type::operator TypeId() const noexcept {
        return id;
    }

    PolymorphicType::PolymorphicType(const Type &type)
    : PolymorphicType(type, nullptr)
    {}

    PolymorphicType::PolymorphicType(const Type &type, const PolymorphicType * parent)
    : type(type), parent(parent)
    {}

    const char *PolymorphicType::name() const noexcept {
        return type.name();
    }

    size_t PolymorphicType::hash_code() const noexcept {
        return type.hash_code();
    }

    bool PolymorphicType::operator==(const PolymorphicType &other) const noexcept {
        return type == other.type
            && (parent == other.parent)
            && (parent ? *parent == *other.parent : true);
    }

    bool PolymorphicType::hasParent() const noexcept {
        return parent != nullptr;
    }
}