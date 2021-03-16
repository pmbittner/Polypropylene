//
// Created by Paul Bittner on 16.03.2021.
//

#include "polypropylene/memory/Allocator.h"

namespace PAX {
    Allocator::Allocator(const std::string &name) : name(name) {}

    Allocator::~Allocator() = default;

    const std::string & Allocator::getName() const {
        return name;
    }
}