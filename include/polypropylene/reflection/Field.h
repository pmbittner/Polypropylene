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
    using FieldFlag = uint32_t;

    /**
     * Reflection class to access fields of classes or global variables by name.
     */
    struct Field {
        /**
         * Feel free to extend the possible Flags for fields either by
         * extending them directly in this file, or specifying them in your
         * project:
         *
         * namespace MyCoolProject::FieldFlags {
         *   // custom additional flags for fields
         *   constexpr FieldFlag IsPublic = ::PAX::Field::CustomFlagsBegin;
         *   constexpr FieldFlag IsConst  = 2 * IsPublic;
         *   constexpr FieldFlag IsFancy  = 2 * IsConst;
         * }
         *
         * That way up to 32 flags are supported from which one is already defined by Field (IsMandatory).
         * You may only use powers of two as values to allow bitwise conjunction of flags:
         *   PAX::Field::IsMandatory | MyCoolProject::FieldFlags::IsPublic
         */
        constexpr static FieldFlag None = 0;
        constexpr static FieldFlag IsMandatory = 1;
        constexpr static FieldFlag CustomFlagsBegin = 2 * IsMandatory;

        TypeHandle type;
        std::string name;
        void * data;
        FieldFlag flags;

        /**
         * Creates a field handle pointing to the concrete variable.
         * For simple creation you may use the "paxfield" macro as follows:
         *   int coolInt;
         *   int mandatoryCoolInt;
         *
         *   Field f = Field(paxfield(coolInt));
         *   Field g = Field(paxfield(mandatoryCoolInt), Field::IsMandatory);
         *
         * @param name The name of the field used for its identification. The name may differ from the actual fields name in the code.
         * @param type A TypeHandle identifying the fields type and size.
         * @param data A pointer to the field such that it can be read and written to.
         * @param flags Optional flags for further attribute specification.
         */
        Field(const std::string & name, const TypeHandle & type, void * data, FieldFlag flags = None);

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
