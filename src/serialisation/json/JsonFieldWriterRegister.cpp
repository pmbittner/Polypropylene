//
// Created by Paul Bittner on 17.03.2020.
//

#include "polypropylene/serialisation/json/JsonFieldWriterRegister.h"

namespace PAX::Json {
    JsonFieldWriterRegister::JsonFieldWriterRegister() = default;

    JsonFieldWriterRegister& JsonFieldWriterRegister::Instance() {
        static JsonFieldWriterRegister instance;
        return instance;
    }

    bool JsonFieldWriterRegister::hasWriterForType(const PAX::TypeId &type) const {
        return writers.find(type) != writers.end();
    }

    bool JsonFieldWriterRegister::registerWriter(const PAX::TypeId &type, const IJsonFieldWriter * writer) {
        if (!hasWriterForType(type)) {
            writers.insert({type, writer});
            return true;
        }
        return false;
    }

    const IJsonFieldWriter * JsonFieldWriterRegister::getWriterFor(const PAX::TypeId &type) const {
        const auto &it = writers.find(type);
        if (it != writers.end()) {
            return it->second;
        }

        PAX_THROW_RUNTIME_ERROR("No IJsonFieldWriter registered for type: " << type.name());
    }
}