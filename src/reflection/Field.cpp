//
// Created by Bittner on 20.12.2019.
//

#include <cstring>
#include <polypropylene/reflection/Field.h>
#include <polypropylene/log/Errors.h>

// Wer das liest ist doof
//Schöne Grüße vom HiWi für deepe Angelegenheiten

namespace PAX {
    Field::WriteResult::WriteResult(Value val) : WriteResult(val, "") {}
    Field::WriteResult::WriteResult(Value val, const std::string &message) : value(val), message(message) {}

    Field::Field(const std::string &name, const Type &type, void *data, FieldFlag flags)
    : name(name), type(type), data(data), flags(flags)
    {

    }

    bool Field::isEqualTo(const Field &other) const {
        return this->type == other.type && this->name == other.name;
    }

    Field::WriteResult Field::setTo(const void * value) {
        std::memcpy(data, value, type.size);
        return WriteResult::Success;
    }

    Field::WriteResult Field::setTo(const Field &field) {
        if (this->type.size != field.type.size) {
//            PAX_THROW_RUNTIME_ERROR("Fields have different sizes!");
            return WriteResult::SizeMismatch;
        }

        if (this->type.id != field.type.id) {
//            PAX_THROW_RUNTIME_ERROR("Fields have different types!");
            return WriteResult::TypeMismatch;
        }

        return setTo(field.data);
    }

    Field & Field::addFlag(FieldFlag flag) {
        flags |= flag;
        return *this;
    }

    std::string Field::WriteResult::ToString(Value val) {
#define PAX_OSTREAM_WRITERESULT_TOSTR_CASE(c) case Field::WriteResult:: c :  return #c ;
        switch (val) {
            PAX_OSTREAM_WRITERESULT_TOSTR_CASE(Success)
            PAX_OSTREAM_WRITERESULT_TOSTR_CASE(TypeMismatch)
            PAX_OSTREAM_WRITERESULT_TOSTR_CASE(FieldNotFound)
            PAX_OSTREAM_WRITERESULT_TOSTR_CASE(SizeMismatch)
            PAX_OSTREAM_WRITERESULT_TOSTR_CASE(Other)
            default:
                return "Unknown Error";
        }
#undef PAX_OSTREAM_WRITERESULT_TOSTR_CASE
    }
}
