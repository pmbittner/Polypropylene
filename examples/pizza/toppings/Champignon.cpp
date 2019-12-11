//
// Created by Bittner on 06.12.2019.
//

#include "Champignon.h"

namespace Proteus::Examples {
    PAX_PROPERTY_SOURCE(Champignon, PAX_PROPERTY_IS_CONCRETE)

    Proteus::Examples::Champignon::This * Champignon::createFromProvider(::PAX::ContentProvider & ) {
        return new Champignon();
    }

    void Champignon::initializeFromProvider(PAX::ContentProvider & c) {
        Super::initializeFromProvider(c);
    }

    std::string Champignon::yummy() {
        return "Champignon";
    }
}