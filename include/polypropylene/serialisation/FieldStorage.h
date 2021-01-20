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

        /**
         * @return True, iff this storage has a field with the given name.
         */
        PAX_NODISCARD virtual bool has(const std::string & name) const = 0;

        /**
         * Writes to the given field, the value that is stored in this storage for the given field's name.
         * The inverse operation is readFrom.
         * @param field The field that should be written to. It is identified by its name.
         * @param variables A register of variables that may be used by values for fields in this storage.
         * @return True, iff a value was written to the given field.
         *         False, there is no value for the given field in this storage or there is a type mismatch.
         */
        PAX_NODISCARD virtual Field::WriteResult writeTo(Field &field, const VariableRegister & variables) const = 0;

        /**
         * Stores the value from the given field in this storage under the given field's name.
         * This method does not take ownership.
         * The inverse operation is writeTo.
         * @param field The field to serialize (i.e., to store in this storage).
         * @return True, iff the field could be stored in this FieldStorage.
         */
        virtual bool readFrom(const Field & field) = 0;

        PAX_NODISCARD virtual std::string toString() const = 0;
    };
}

#endif //POLYPROPYLENE_FIELDSTORAGE_H
