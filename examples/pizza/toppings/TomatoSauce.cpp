//
// Created by Bittner on 06.12.2019.
//

#include <polypropylene/reflection/ClassMetadata.h>
#include "TomatoSauce.h"

namespace PAX::Examples {
    PAX_PROPERTY_SOURCE(TomatoSauce, PAX_PROPERTY_IS_CONCRETE)

    ClassMetadata TomatoSauce::getMetadata() {
        ClassMetadata m = Super::getMetadata();
        m.add({"scoville", paxtypeof(int), &scoville});
        return m;
    }

    std::string TomatoSauce::yummy() {
        return std::to_string(scoville) + " scoville tomato sauce";
    }

    void TomatoSauce::setSpicyness(float scoville) {
        this->scoville = scoville;
    }
}