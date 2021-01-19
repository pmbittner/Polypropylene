//
// Created by Bittner on 20.12.2019.
//

#ifndef POLYPROPYLENE_CLASSMETADATA_H
#define POLYPROPYLENE_CLASSMETADATA_H

#include <vector>
#include <polypropylene/definitions/Definitions.h>

#include "Field.h"

namespace PAX {
    class ClassMetadata {
        std::string className;
        std::vector<Field> fields;

    public:
        explicit ClassMetadata(const std::string & className);
        ClassMetadata(const ClassMetadata & other);
        ClassMetadata(ClassMetadata && other) noexcept;

        Field& add(Field && field);
        PAX_NODISCARD bool contains(const std::string & name) const;
        PAX_NODISCARD Field & get(const std::string & name);
        PAX_NODISCARD const Field & get(const std::string & name) const;
        PAX_NODISCARD std::vector<Field> & getFields();
        PAX_NODISCARD const std::vector<Field> & getFields() const;
        PAX_NODISCARD const std::string & getName() const;

        void writeTo(ClassMetadata & other) const;
    };
}

#endif //POLYPROPYLENE_CLASSMETADATA_H
