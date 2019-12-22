//
// Created by Bittner on 20.12.2019.
//

#include <cstring>
#include <polypropylene/reflection/Field.h>
#include <polypropylene/log/Errors.h>

// Wer das liest ist doof
//Schöne Grüße vom HiWi für deepe Angelegenheiten

namespace PAX {
    Field::Field(const std::string &name, const PAX::TypeHandle &type, void *data, PAX::Field::Flag flags)
    : name(name), type(type), data(data), flag(flags)
    {

    }

    bool Field::isEqualTo(const Field &other) const {
        return this->type == other.type && this->name == other.name;
    }

    void Field::setFrom(const void * value) {
        std::memcpy(data, value, type.size);
    }

    void Field::setFrom(const Field &field) {
        if (this->type.size != field.type.size) {
            PAX_THROW_RUNTIME_ERROR("Fields have different sizes!");
        }

        if (this->type.typeindex != field.type.typeindex) {
            PAX_THROW_RUNTIME_ERROR("Fields have different types!");
        }

        setFrom(field.data);
    }
}
