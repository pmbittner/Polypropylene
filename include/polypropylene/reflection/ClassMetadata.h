//
// Created by Bittner on 20.12.2019.
//

#ifndef POLYPROPYLENE_CLASSMETADATA_H
#define POLYPROPYLENE_CLASSMETADATA_H

#include <vector>
#include <polypropylene/macros/Definitions.h>

#include "Field.h"

namespace PAX {
    class ClassMetadata {
        std::vector<Field> fields;

    public:
        ClassMetadata();
        ClassMetadata(ClassMetadata && other) noexcept;

        void add(const Field & field);
        PAX_NODISCARD bool contains(const std::string & name) const;
        PAX_NODISCARD Field get(const std::string & name) const;
        PAX_NODISCARD std::vector<Field> & getFields();
    };
}

#endif //POLYPROPYLENE_CLASSMETADATA_H
