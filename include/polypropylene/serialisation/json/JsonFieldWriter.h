//
// Created by Paul Bittner on 17.03.2020.
//

#ifndef PAXENGINE3_JSONFIELDWRITER_H
#define PAXENGINE3_JSONFIELDWRITER_H

#ifdef PAX_WITH_JSON

#include <sstream>

#include "polypropylene/reflection/Field.h"
#include "JsonTypeConverter.h"

// TODO: Avoid
#include "nlohmann/Json.h"

namespace PAX {
    namespace Json {
        class IJsonFieldWriter {
        public:
            /**
             * Writes the data in the json object with key to the given field corresponding to the field's name.
             * @param j json to read data from
             * @param field The data pointed to by the field is set to the value of the json with the name of the field.
             * @return True if writing was successful. Returns false if this parser cannot write to the given field.
             * This can happen if the type of the field does not correspond to the type this parser can read.
             */
            PAX_NODISCARD virtual Field::WriteResult loadIntoField(const nlohmann::json &j, Field &field) const = 0;

            virtual Field::WriteResult loadIntoVector(const nlohmann::json &j, Field &field) const = 0;

            /**
             * Writes the data given in the field to the json.
             * @param field The field that should be written to json.
             * @param j The json object whose value for key being the name of the field should be set.
             * @return True if the json could be written. Returns false if this parser cannot read the given field.
             * This can happen if the type of the field does not correspond to the type this parser can read.
             */
            PAX_NODISCARD virtual bool loadIntoJson(const Field &field, nlohmann::json &j) const = 0;
        };

        template<class T>
        class JsonFieldWriter : public IJsonFieldWriter {
        public:
            PAX_NODISCARD Field::WriteResult loadIntoField(const nlohmann::json &j, Field &field) const override {
                // TODO: Make this a method of field?
                if (field.type == paxtypeof(T)) {
                    *static_cast<T *>(field.data) = std::move(TypeConverter<nlohmann::json, T>::convertTo(j));
                    return Field::WriteResult::Success;
                }

                return Field::WriteResult::TypeMismatch;
            }

            PAX_NODISCARD Field::WriteResult loadIntoVector(const nlohmann::json &j, Field &field) const override {
                using Vec = std::vector<T>;

                // TODO: Make this a method of field? This could avoid having to include json.h
                if (field.type == paxtypeof(T) && j.is_array()) {
                    Vec* vec = static_cast<Vec *>(field.data);
                    for (const nlohmann::json & child : j) {
                        vec->emplace_back(TypeConverter<nlohmann::json, T>::convertTo(child));
                    }

                    return Field::WriteResult::Success;
                }

                return Field::WriteResult::TypeMismatch;
            }

            PAX_NODISCARD bool loadIntoJson(const Field & field, nlohmann::json & j) const override {
                if (field.type == paxtypeof(T)) {
                    const T* data = static_cast<T*>(field.data);
                    j.emplace(field.name, TypeConverter<nlohmann::json, T>::convertFrom(*data));
                    return true;
                }

                return false;
            }
        };
    }
}

#endif // PAX_WITH_JSON

#endif //PAXENGINE3_JSONFIELDWRITER_H
