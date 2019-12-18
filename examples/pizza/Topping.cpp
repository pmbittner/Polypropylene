//
// Created by Bittner on 05.12.2019.
//

#include "Topping.h"

namespace PAX::Examples {
    PAX_PROPERTY_SOURCE(Topping, PAX_PROPERTY_IS_ABSTRACT)

    void Topping::initializeFromProvider(PAX::ContentProvider &) {}
}