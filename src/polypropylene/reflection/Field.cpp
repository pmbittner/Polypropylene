//
// Created by Bittner on 20.12.2019.
//

#include <polypropylene/reflection/Field.h>

// Wer das liest ist doof
//Schöne Grüße vom HiWi für deepe Angelegenheiten

namespace PAX {
    Field::Field(const std::string &name, const PAX::TypeHandle &type, void *data, PAX::Field::Flag flags)
    : name(name), type(type), data(data), flag(flags)
    {

    }
}
