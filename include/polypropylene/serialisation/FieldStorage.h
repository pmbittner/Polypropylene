//
// Created by Bittner on 21.12.2019.
//

#ifndef POLYPROPYLENE_FIELDSTORAGE_H
#define POLYPROPYLENE_FIELDSTORAGE_H

#include <polypropylene/definitions/Definitions.h>
#include <polypropylene/reflection/Field.h>
#include <polypropylene/reflection/VariableRegister.h>

namespace PAX {
    class FieldStorage {
    public:
        FieldStorage() = default;
        virtual ~FieldStorage() = 0;

        PAX_NODISCARD virtual bool has(const std::string &name) const = 0;
        PAX_NODISCARD virtual VariableHierarchy getResourceParametersFor(const std::string &name) const = 0;
        PAX_NODISCARD virtual bool writeTo(Field &field, const VariableRegister &variables) const = 0;
        virtual void readFrom(Field &field) = 0;
    };
}

#endif //POLYPROPYLENE_FIELDSTORAGE_H
