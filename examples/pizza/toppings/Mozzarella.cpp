//
// Created by Bittner on 06.12.2019.
//

#include "Mozzarella.h"

namespace PAX::Examples {
    PAX_PROPERTY_SOURCE(Mozzarella, PAX_PROPERTY_IS_CONCRETE)

    Mozzarella * Mozzarella::createFromProvider(::PAX::ContentProvider & ) {
        return new Mozzarella();
    }

    void Mozzarella::initializeFromProvider(PAX::ContentProvider & c) {
        Super::initializeFromProvider(c);
    }

    std::string Mozzarella::yummy() {
        return Super::yummy() + " Mozzarella";
    }
}