//
// Created by Paul on 02.03.2019.
//

#ifndef POLYPROPYLENE_JSONFIELDSTORAGE_H
#define POLYPROPYLENE_JSONFIELDSTORAGE_H

#ifdef PAX_WITH_JSON

#include "polypropylene/serialisation/ClassMetadataSerialiser.h"
// TODO: Find a way to include JsonFwd.h here instead of Json.h
#include "polypropylene/serialisation/json/Json.h"
#include "polypropylene/serialisation/json/JsonParser.h"

namespace PAX::Json {
    class JsonFieldStorage : public FieldStorage {
        nlohmann::json & node;
        const JsonParserRegister & parsers;

    public:
        explicit JsonFieldStorage(nlohmann::json &node, const JsonParserRegister & jsonParserRegister);
        ~JsonFieldStorage() override;

        PAX_NODISCARD std::string getValue(const std::string &key, const VariableRegister & variables) const;

        PAX_NODISCARD bool has(const std::string &name) const override;
        PAX_NODISCARD bool writeTo(Field & field, const VariableRegister &variables) const override;
        bool readFrom(const Field & field) override;

        void clear();

        PAX_NODISCARD std::string toString() const override;
    };
}

#endif // PAX_WITH_JSON

#endif //POLYPROPYLENE_JSONFIELDSTORAGE_H
