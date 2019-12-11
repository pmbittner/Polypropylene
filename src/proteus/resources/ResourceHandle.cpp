//
// Created by Paul on 03.10.2017.
//

#include <proteus/resources/ResourceHandle.h>

namespace PAX {
    ResourceHandle::ResourceHandle() = default;

    ResourceHandle::~ResourceHandle() {
        delete _signature;
    }
}