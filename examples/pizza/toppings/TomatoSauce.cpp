//
// Created by Bittner on 06.12.2019.
//

#include <polypropylene/reflection/ClassMetadata.h>
#include "TomatoSauce.h"

namespace PAX::Examples {
    PAX_PROPERTY_IMPL(TomatoSauce)

    TomatoSauce::TomatoSauce() = default;
    TomatoSauce::TomatoSauce(int scoville) : scoville(scoville) { }

    ClassMetadata TomatoSauce::getMetadata() {
        ClassMetadata m = Super::getMetadata();
        m.add(paxfieldof(scoville)).flags = Field::IsMandatory;
        return m;
    }

    std::string TomatoSauce::yummy() {
        return std::to_string(scoville) + " scoville tomato sauce";
    }

    int TomatoSauce::getScoville() const {
        return scoville;
    }
}