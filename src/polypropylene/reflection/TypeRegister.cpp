//
// Created by paul on 07.01.19.
//

#include <polypropylene/reflection/TypeRegister.h>

namespace PAX {
    TypeRegister::TypeRegister() = default;

    void TypeRegister::set(const TypeHandle &type, void *factory) {
        factories[type] = factory;
    }
}