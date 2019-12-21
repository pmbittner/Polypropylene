//
// Created by Bittner on 21/05/2019.
//

#ifndef POLYPROPYLENE_JSONPARSER_H
#define POLYPROPYLENE_JSONPARSER_H

#ifdef PAX_WITH_JSON

#include "JsonUtil.h"
#include "polypropylene/serialisation/TryParser.h"
#include "polypropylene/reflection/TypeMap.h"
#include "polypropylene/reflection/Field.h"

namespace PAX {
    class IJsonParser {
    public:
        PAX_NODISCARD virtual bool loadIntoField(const nlohmann::json & j, Field & field) const = 0;
    };

    class JsonParserRegister {
        static JsonParserRegister * instance;
        TypeMap<IJsonParser*> parsers;

    public:
        void registerParser(const PAX::TypeHandle & type, IJsonParser * parser);
        PAX_NODISCARD static JsonParserRegister * Instance();
        PAX_NODISCARD IJsonParser * getParserFor(const PAX::TypeHandle & type) const;
    };

    template<class T>
    class TryParser<nlohmann::json, T> {
    public:
        PAX_NODISCARD static T tryParse(const nlohmann::json & f) {
            return String::tryParse<T>(JsonToString(f));
        }
    };

    template<class T>
    class JsonParser : public IJsonParser {
    public:
        PAX_NODISCARD bool loadIntoField(const nlohmann::json & j, Field & field) const override {
            if (field.type == paxtypeof(T)) {
                // TODO: Move?
                *static_cast<T*>(field.data) = TryParser<nlohmann::json, T>::tryParse(j);
                return true;
            }

            return false;
        }
    };

    namespace Json {
        template<typename T>
        PAX_NODISCARD T tryParse(const nlohmann::json & j) {
            return TryParser<nlohmann::json, T>::tryParse(j);
        }
    }

#define PAX_SPECIALIZE_JSONTRYPARSE_HEADER(type) \
            template<> \
            class TryParser<nlohmann::json, type> { \
            public: \
                PAX_NODISCARD static type tryParse(const nlohmann::json & j); \
            };
}

#else // !PAX_WITH_JSON
#define PAX_SPECIALIZE_JSONTRYPARSE_HEADER(type)
#endif // PAX_WITH_JSON

#endif //POLYPROPYLENE_JSONPARSER_H
