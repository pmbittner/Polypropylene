//
// Created by Paul on 02.03.2019.
//

#include "polypropylene/serialisation/json/JsonFieldStorage.h"
#include "polypropylene/serialisation/json/nlohmann/Json.h"

namespace PAX::Json {
    JsonFieldStorage::JsonFieldStorage(nlohmann::json &node, const JsonFieldWriterRegister & jsonFieldWriterRegister)
            : FieldStorage(), node(node), writers(jsonFieldWriterRegister) {}

    JsonFieldStorage::~JsonFieldStorage() = default;

    bool JsonFieldStorage::has(const std::string &name) const {
        return node.count(name) > 0;
    }

    std::string JsonFieldStorage::getValue(const std::string &key, const VariableRegister & variables) const {
        return VariableResolver::resolveVariables(JsonToString(node[key]), variables);
    }

    Field::WriteResult JsonFieldStorage::writeTo(Field & field, const VariableRegister &variables) const {
        if (!has(field.name)) {
            std::stringstream ss;
            ss << "Json object\n" << JsonToString(node) << "\ndoes not have a value for " << field.name << "!";
            return Field::WriteResult(Field::WriteResult::FieldNotFound, ss.str());
        }

        const IJsonFieldWriter * writer = writers.getWriterFor(field.type.id);
        if (writer) {
            const nlohmann::json value = StringToJson(getValue(field.name, variables));
            if (field.flags & Field::IsVector) {
                return writer->loadIntoVector(value, field);
            } else {
                return writer->loadIntoField(value, field);
            }
        } else {
            std::stringstream ss;
            ss << "Could not write to field of type \"" << field.type.name() << "\" because no IJsonFieldWriter is registered for it!";
            return Field::WriteResult(Field::WriteResult::Other, ss.str());
        }
    }

    bool JsonFieldStorage::readFrom(const Field & field) {
        clear();

        const IJsonFieldWriter * writer = writers.getWriterFor(field.type.id);
        if (writer) {
            return writer->loadIntoJson(field, node);
        } else {
            PAX_THROW_RUNTIME_ERROR("Could not read field of type \"" << field.type.name() << "\" because no IJsonFieldWriter is registered for it!");
        }

        return false;
    }

    void JsonFieldStorage::clear() {
        node.clear();
    }

    std::string JsonFieldStorage::toString() const {
        return JsonToString(node);
    }
}
