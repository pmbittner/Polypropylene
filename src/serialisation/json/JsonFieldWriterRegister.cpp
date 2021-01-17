//
// Created by Paul Bittner on 17.03.2020.
//

#include "polypropylene/serialisation/json/JsonFieldWriterRegister.h"

namespace PAX::Json {
#define JSONREGISTER_REGISTERPRIMITIVE(type) \
    { \
        static JsonFieldWriter<type> writer; \
        registerWriter(paxtypeid(type), &writer); \
    }

#define JSONREGISTER_REGISTERPRIMITIVE_WITHUNSIGNED(type) \
    JSONREGISTER_REGISTERPRIMITIVE(type) \
    JSONREGISTER_REGISTERPRIMITIVE(unsigned type)

    JsonFieldWriterRegister::JsonFieldWriterRegister() {
        // register some defaults
        JSONREGISTER_REGISTERPRIMITIVE(bool)
        JSONREGISTER_REGISTERPRIMITIVE(char)
        JSONREGISTER_REGISTERPRIMITIVE_WITHUNSIGNED(short)
        JSONREGISTER_REGISTERPRIMITIVE_WITHUNSIGNED(int)
        JSONREGISTER_REGISTERPRIMITIVE_WITHUNSIGNED(long)
        JSONREGISTER_REGISTERPRIMITIVE_WITHUNSIGNED(long long)
        JSONREGISTER_REGISTERPRIMITIVE(float)
        JSONREGISTER_REGISTERPRIMITIVE(double)

        // not really a primitive but works
        JSONREGISTER_REGISTERPRIMITIVE(std::string)
        JSONREGISTER_REGISTERPRIMITIVE(Path)
    }

#undef JSONREGISTER_REGISTERPRIMITIVE_WITHUNSIGNED
#undef JSONREGISTER_REGISTERPRIMITIVE

    void JsonFieldWriterRegister::registerWriter(const PAX::TypeId &type, const IJsonFieldWriter * writer) {
        writers.insert({type, writer});
    }

    const IJsonFieldWriter * JsonFieldWriterRegister::getWriterFor(const PAX::TypeId &type) const {
        const auto &it = writers.find(type);
        if (it != writers.end()) {
            return it->second;
        }

        PAX_THROW_RUNTIME_ERROR("No IJsonFieldWriter registered for type: " << type.name());
    }
}