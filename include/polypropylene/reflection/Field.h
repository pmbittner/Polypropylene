//
// Created by Bittner on 20.12.2019.
//

#ifndef POLYPROPYLENE_FIELD_H
#define POLYPROPYLENE_FIELD_H

#include <string>
#include <vector>
#include "polypropylene/definitions/Definitions.h"
#include "polypropylene/reflection/Type.h"

#define paxfieldalias(name, field) ::PAX::Internal::FieldCreator<decltype(field)>::createField(name, &field)
#define paxfieldof(field) paxfieldalias(#field, field)

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
        constexpr static FieldFlag NoFlag = 0;
        constexpr static FieldFlag IsMandatory = 1;
        constexpr static FieldFlag IsVector = 2;
        constexpr static FieldFlag CustomFlagsBegin = 2 * IsVector;

        Type type;
        std::string name;
        void *data;
        FieldFlag flags;

        struct WriteResult {
            enum Value {
                Success = 0,
                FieldNotFound,
                TypeMismatch,
                SizeMismatch,
                Other
            };

            const Value value;
            const std::string message;

            PAX_IMPLICIT WriteResult(Value val);

            WriteResult(Value val, const std::string &message);

            /**
             * Converts a WriteResult::Value to string.
             * (Overloading ostream>> caused trouble. :( )
             * @param v
             * @return
             */
            static std::string ToString(Value v);
        };

        /**
         * Creates a field handle pointing to the concrete variable.
         * For simple creation you may use the "paxfieldof" macro as follows:
         *   int coolInt;
         *   int mandatoryCoolInt;
         *
         *   Field f = paxfieldof(coolInt);
         *   Field g = paxfieldof_flagged(mandatoryCoolInt, Field::IsMandatory);
         *
         * @param name The name of the field used for its identification. The name may differ from the actual fields name in the code.
         * @param type A TypeHandle identifying the fields type and size.
         * @param data A pointer to the field such that it can be read and written to.
         * @param flags Optional flags for further attribute specification.
         */
        Field(const std::string &name, const Type &type, void *data, FieldFlag flags = NoFlag);

        /**
         * Checks if this field describes the same field as the other field by considering name and type equality.
         * Both fields may point to different data, though.
         * @param other The field to which equality should be checked.
         * @return Returns true, if name, size, and type of the given field are equal to the respective values of this field.
         */
        PAX_NODISCARD bool isEqualTo(const Field &other) const;

        /**
         * Writes the data pointed to by 'value' to this field.
         * Be careful and ensure that sizeof(*value) is the same as the size of this field!
         * @param value Pointer to data to read from.
         */
        WriteResult setTo(const void *value);

        /**
         * Sets the data of this field to the data of the given field.
         * @param field The field whose 'data' should be copied to this field.
         */
        WriteResult setTo(const Field &field);

        Field &addFlag(FieldFlag flag);
    };
}

#ifdef PAX_WITH_JSON
#include "polypropylene/serialisation/json/JsonFieldWriterRegister.h"
#endif

namespace PAX::Internal {
#ifdef PAX_WITH_JSON
    template<typename T>
    bool registerJsonFieldWriterForType() {
        static Json::JsonFieldWriter<T> writer;
        return Json::JsonFieldWriterRegister::Instance().registerWriter(paxtypeid(T), &writer);
    }
#endif

    template<typename T>
    struct FieldCreator {
        static Field createField(const std::string & name, void * data) {
#ifdef PAX_WITH_JSON
            static bool dummy = registerJsonFieldWriterForType<T>();
#endif
            return Field(name, paxtypeof(T), data, ::PAX::Field::NoFlag);
        }
    };

    template<typename T>
    struct FieldCreator<std::vector<T>> {
        static Field createField(const std::string &name, void * data) {
#ifdef PAX_WITH_JSON
            static bool dummy = registerJsonFieldWriterForType<T>();
#endif
            return Field(name, paxtypeof(T), data, ::PAX::Field::IsVector);
        }
    };
}

#endif //POLYPROPYLENE_FIELD_H
