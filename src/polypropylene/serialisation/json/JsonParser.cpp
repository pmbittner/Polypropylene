//
// Created by Bittner on 20.12.2019.
//

#include <polypropylene/serialisation/json/JsonParser.h>
#include <polypropylene/log/Errors.h>

namespace PAX::Json {
#define JSONPARSERREGISTER_REGISTERPRIMITIVE(type) \
    { \
        static JsonParser<type> parser; \
        registerParser(paxtypeof(type), &parser); \
    }

#define JSONPARSERREGISTER_REGISTERPRIMITIVE_WITHUNSIGNED(type) \
    JSONPARSERREGISTER_REGISTERPRIMITIVE(type) \
    JSONPARSERREGISTER_REGISTERPRIMITIVE(unsigned type)

    JsonParserRegister::JsonParserRegister() {
        // register some defaults
        JSONPARSERREGISTER_REGISTERPRIMITIVE(bool)
        JSONPARSERREGISTER_REGISTERPRIMITIVE(char)
        JSONPARSERREGISTER_REGISTERPRIMITIVE_WITHUNSIGNED(short)
        JSONPARSERREGISTER_REGISTERPRIMITIVE_WITHUNSIGNED(int)
        JSONPARSERREGISTER_REGISTERPRIMITIVE_WITHUNSIGNED(long)
        JSONPARSERREGISTER_REGISTERPRIMITIVE_WITHUNSIGNED(long long)
        JSONPARSERREGISTER_REGISTERPRIMITIVE(float)
        JSONPARSERREGISTER_REGISTERPRIMITIVE(double)

        // not really a primitive but works
        JSONPARSERREGISTER_REGISTERPRIMITIVE(std::string)
    }

#undef JSONPARSERREGISTER_REGISTERPRIMITIVE_WITHUNSIGNED
#undef JSONPARSERREGISTER_REGISTERPRIMITIVE

    void JsonParserRegister::registerParser(const PAX::TypeHandle & type, IJsonParser *parser) {
        parsers.insert({type, parser});
    }

    IJsonParser * JsonParserRegister::getParserFor(const PAX::TypeHandle & type) const {
        const auto & it = parsers.find(type);
        if (it != parsers.end()) {
            return it->second;
        }

        PAX_THROW_RUNTIME_ERROR("No IJsonParser registered for type: " << type.name());
    }
}