//
// Created by Bittner on 20.12.2019.
//

#ifndef POLYPROPYLENE_FIELD_H
#define POLYPROPYLENE_FIELD_H

#include <string>
#include <polypropylene/definitions/Definitions.h>
#include <polypropylene/reflection/TypeHandle.h>

#define paxfieldpointer(field) paxtypeof(decltype(field)), &field
#define paxfield(field) #field, paxfieldpointer(field)

namespace PAX {
    struct Field {
        enum Flag : uint32_t {
            None = 0,
            IsMandatory = 1,
            //IsResource = 2 // For future use in engine
        };

        TypeHandle type;
        std::string name;
        Flag flag;
        void * data;

        Field(const std::string & name, const TypeHandle & type, void * data, Flag flags = None);

        /**
         * Checks if this field describes the same field as the other field by considering name and type equality.
         * Both fields may point to different data, though.
         * @param other The field to which equality should be checked.
         * @return Returns true, if name, size, and type of the given field are equal to the respective values of this field.
         */
        PAX_NODISCARD bool isEqualTo(const Field & other) const;

        /**
         * Writes the data pointed to by 'value' to this field.
         * Be careful and ensure that sizeof(*value) is the same as the size of this field!
         * @param value Pointer to data to read from.
         */
        void setFrom(const void * value);

        /**
         * Sets the data of this field to the data of the given field.
         * @param field The field whose 'data' should be copied to this field.
         */
        void setFrom(const Field & field);
    };
}

#endif //POLYPROPYLENE_FIELD_H
