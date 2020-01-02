//
// Created by Bittner on 21.12.2019.
//

#ifndef POLYPROPYLENE_FIELDSTORAGE_H
#define POLYPROPYLENE_FIELDSTORAGE_H

#include <polypropylene/definitions/Definitions.h>
#include <polypropylene/reflection/Field.h>
#include <polypropylene/reflection/VariableRegister.h>

namespace PAX {
    /**
     * Interface for representing persistent serialisation data, typically present on the hard drive.
     * Fields can be read from or written to a FieldStorage for serialisation.
     * For example, @ref JsonFieldStorage translates fields to json files.
     */
    class FieldStorage {
    public:
        FieldStorage() = default;
        virtual ~FieldStorage() = 0;

        PAX_NODISCARD virtual bool has(const std::string & name) const = 0;
        PAX_NODISCARD virtual bool writeTo(Field &field, const VariableRegister & variables) const = 0;
        virtual bool readFrom(const Field & field) = 0;
    };
}

#endif //POLYPROPYLENE_FIELDSTORAGE_H
