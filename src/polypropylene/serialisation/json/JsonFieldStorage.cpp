//
// Created by Paul on 02.03.2019.
//

#include <polypropylene/serialisation/json/JsonFieldStorage.h>
#include <polypropylene/serialisation/json/JsonParser.h>
#include "polypropylene/log/Errors.h"

namespace PAX::Json {
    JsonFieldStorage::JsonFieldStorage(nlohmann::json &node, const JsonParserRegister & jsonParserRegister)
            : FieldStorage(), node(node), parsers(jsonParserRegister) {}

    JsonFieldStorage::~JsonFieldStorage() = default;

    bool JsonFieldStorage::has(const std::string &name) const {
        return node.count(name) > 0;
    }

    std::string JsonFieldStorage::getValue(const std::string &key, const VariableRegister & variables) const {
        return VariableResolver::resolveVariables(JsonToString(node[key]), variables);
    }

    bool JsonFieldStorage::writeTo(Field & field, const VariableRegister &variables) const {
        nlohmann::json j = StringToJson(getValue(field.name, variables));

        const IJsonParser * parser = parsers.getParserFor(field.type.id);
        if (parser) {
            return parser->loadIntoField(j, field);
        } else {
            PAX_THROW_RUNTIME_ERROR("Could not write to field of type \"" << field.type.name() << "\" because no IJsonParser is registered for it!");
        }

        return true;
    }

    bool JsonFieldStorage::readFrom(const Field & field) {
        clear();

        const IJsonParser * parser = parsers.getParserFor(field.type.id);
        if (parser) {
            return parser->loadIntoJson(field, node);
        } else {
            PAX_THROW_RUNTIME_ERROR("Could not read field of type \"" << field.type.name() << "\" because no IJsonParser is registered for it!");
        }

        return true;
    }

    void JsonFieldStorage::clear() {
        node.clear();
    }
}
