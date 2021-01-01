//
// Created by Bittner on 06.12.2019.
//

#include "Pizza.h"
#include "BakedEvent.h"

namespace PAX::Examples {
    void Pizza::yummy() {
        std::string message = "Pizza has:\n";
        for (Topping * t : get<Topping>()) {
            message += "  - " + t->yummy() + "\n";
        }

        PAX_LOG(PAX::Log::Level::Info, message);
    }

    void Pizza::bake() {
        PAX_LOG(PAX::Log::Level::Info, "Pizza gets baked... ~~~~");
        BakedEvent bakedEvent;
        getEventService().fire(bakedEvent);
    }
}