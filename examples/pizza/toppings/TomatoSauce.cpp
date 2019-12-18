//
// Created by Bittner on 06.12.2019.
//

#include "TomatoSauce.h"

namespace PAX::Examples {
    PAX_PROPERTY_SOURCE(TomatoSauce, PAX_PROPERTY_IS_CONCRETE)

    TomatoSauce::This * TomatoSauce::createFromProvider(PAX::ContentProvider & ) {
        return new TomatoSauce();
    }

    void TomatoSauce::initializeFromProvider(PAX::ContentProvider & c) {
        if (std::optional<int> scov = c.get<int>("scoville")) {
            scoville = scov.value();
        }

        Super::initializeFromProvider(c);
    }

    std::string TomatoSauce::yummy() {
        return std::to_string(scoville) + " scoville tomato sauce";
    }

    void TomatoSauce::setSpicyness(float scoville) {
        this->scoville = scoville;
    }
}