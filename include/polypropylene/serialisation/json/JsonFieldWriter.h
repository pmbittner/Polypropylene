//
// Created by Paul Bittner on 17.03.2020.
//

#ifndef PAXENGINE3_JSONFIELDWRITER_H
#define PAXENGINE3_JSONFIELDWRITER_H

#ifdef PAX_WITH_JSON

#include "polypropylene/reflection/Field.h"
#include "JsonParser.h"

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
            PAX_NODISCARD virtual bool loadIntoField(const nlohmann::json &j, Field &field) const = 0;

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
            PAX_NODISCARD bool loadIntoField(const nlohmann::json &j, Field &field) const override {
                if (field.type == paxtypeof(T)) {
                    // TODO: Move?
                    *static_cast<T *>(field.data) = TryParser<nlohmann::json, T>::tryParse(j);
                    return true;
                }

                return false;
            }

            PAX_NODISCARD bool loadIntoJson(const Field & field, nlohmann::json & j) const override {
                if (field.type == paxtypeof(T)) {
                    T* data = static_cast<T*>(field.data);
                    std::stringstream stream;
                    stream << *data;
                    setJsonValue(j, field.name, stream.str());
                    return true;
                }

                return false;
            }
        };
    }
}

#endif // PAX_WITH_JSON

#endif //PAXENGINE3_JSONFIELDWRITER_H
