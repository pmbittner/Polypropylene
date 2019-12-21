//
// Created by Bittner on 06.12.2019.
//

#ifndef PROTEUS_TOMATOSAUCE_H
#define PROTEUS_TOMATOSAUCE_H

#include "../Topping.h"

namespace PAX::Examples {
    class TomatoSauce : public Topping {
        PAX_PROPERTY(TomatoSauce, PAX_PROPERTY_IS_CONCRETE)
        PAX_PROPERTY_DERIVES(Topping)
        PAX_PROPERTY_IS_SINGLE

        int scoville = 0;

        /**
         * Although we require scoville to be set, we provide a private default constructor for deserialisation (Prefabs).
         * We tell the deserialisation then in getMetadata that scoville has to be initialised, i.e., it IsMandatory.
         */
        TomatoSauce();

    public:
        explicit TomatoSauce(int scoville);
        ClassMetadata getMetadata() override;
        std::string yummy() override;
    };
}

#endif //PROTEUS_TOMATOSAUCE_H
