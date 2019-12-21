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

namespace PAX {
    template<class T>
    class TryParser<nlohmann::json, T> {
    public:
        PAX_NODISCARD static T tryParse(const nlohmann::json & f) {
            return ::PAX::String::tryParse<T>(JsonToString(f));
        }
    };

    namespace Json {
        class IJsonParser {
        public:
            PAX_NODISCARD virtual bool loadIntoField(const nlohmann::json &j, Field &field) const = 0;
        };

        /**
         * By default, parsers for all primitive types (bool, char, int, float, ...) are registered.
         * Parsers for arbitrary types can be registered manually.
         */
        class JsonParserRegister {
            TypeMap<IJsonParser *> parsers;

        public:
            JsonParserRegister();

            void registerParser(const PAX::TypeHandle &type, IJsonParser *parser);
            PAX_NODISCARD IJsonParser *getParserFor(const PAX::TypeHandle &type) const;
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
