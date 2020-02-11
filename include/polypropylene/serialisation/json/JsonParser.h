//
// Created by Bittner on 21/05/2019.
//

#ifndef POLYPROPYLENE_JSONPARSER_H
#define POLYPROPYLENE_JSONPARSER_H

#ifdef PAX_WITH_JSON

#include "JsonUtil.h"
#include "polypropylene/stdutils/StringUtils.h"
#include "polypropylene/reflection/TypeMap.h"
#include "polypropylene/reflection/Field.h"

// TODO: Try to avoid this
#include "Json.h"

namespace PAX {
    template<class T>
    class TryParser<nlohmann::json, T> {
    public:
        PAX_NODISCARD static T tryParse(const nlohmann::json & f) {
            return ::PAX::String::tryParse<T>(JsonToString(f));
        }
    };

    template<>
    class TryParser<nlohmann::json, Path> {
    public:
        PAX_NODISCARD static Path tryParse(const nlohmann::json & f);
    };

    namespace Json {
        class IJsonParser {
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
            PAX_NODISCARD virtual bool loadIntoJson(const Field & field, nlohmann::json & j) const = 0;
        };

        /**
         * Register for managing IJsonParsers by types.
         * Each registered parser is assumed to be able to parse the type of data it is registered for.
         * By default, parsers for all primitive types (bool, char, int, float, ...) are registered.
         * Parsers for arbitrary types can be registered manually.
         */
        class JsonParserRegister {
            TypeMap<const IJsonParser *> parsers;

        public:
            JsonParserRegister();

            void registerParser(const PAX::TypeId &type, const IJsonParser * parser);
            PAX_NODISCARD const IJsonParser * getParserFor(const PAX::TypeId &type) const;
        };

        template<class T>
        class JsonParser : public IJsonParser {
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
                    j.emplace(field.name, stream.str());
                    return true;
                }

                return false;
            }
        };

        template<typename T>
        PAX_NODISCARD T tryParse(const nlohmann::json &j) {
            return TryParser<nlohmann::json, T>::tryParse(j);
        }
    }
}

#else // !PAX_WITH_JSON
#define PAX_SPECIALIZE_JSONTRYPARSE_HEADER(type)
#endif // PAX_WITH_JSON

#endif //POLYPROPYLENE_JSONPARSER_H
