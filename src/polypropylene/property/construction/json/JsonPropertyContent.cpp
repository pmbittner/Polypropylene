//
// Created by Paul on 02.03.2019.
//

#include <polypropylene/property/construction/json/JsonPropertyContent.h>
#include <polypropylene/json/JsonParser.h>

namespace PAX::Json {
    JsonPropertyContent::JsonPropertyContent(const nlohmann::json &node)
            : PropertyContent(), node(node) {}

    JsonPropertyContent::~JsonPropertyContent() = default;

    bool JsonPropertyContent::has(const std::string &name) const {
        return node.count(name) > 0;
    }

    std::string JsonPropertyContent::getValue(const std::string &key, const VariableRegister & variables) const {
        return VariableResolver::resolveVariables(JsonToString(node[key]), variables);
    }

    /*
    std::vector<std::string> JsonPropertyContent::getValues(const std::string &key, const VariableRegister & variables) {
        nlohmann::json & keynode = node[key];

        if (keynode.is_array()) {
            std::vector<std::string> values;
            for (auto & it : keynode) {
                values.push_back(VariableResolver::resolveVariables(JsonToString(it), variables));
            }

            return values;
        }

        return {};
    }
     //*/

    static void buildVariableHierarchy(VariableHierarchy &h, const nlohmann::json &node) {
        for (auto &entry : node.items()) {
            if (entry.value().is_string()) {
                h.values[entry.key()] = entry.value();
                //std::cout << "\t" << entry.key() << " -> " << entry.value() << std::endl;
            } else {
                h.children[entry.key()] = VariableHierarchy();
                buildVariableHierarchy(h.children[entry.key()], entry.value());
            }
        }
    }

    VariableHierarchy JsonPropertyContent::getResourceParametersFor(const std::string &name) const {
        VariableHierarchy params;
        buildVariableHierarchy(params, node[name]);
        return params;
    }

    bool JsonPropertyContent::writeTo(Field & field, const VariableRegister &variables) const {
        nlohmann::json j = StringToJson(getValue(field.name, variables));

        IJsonParser * parser = JsonParserRegister::Instance()->getParserFor(field.type);
        if (parser) {
            return parser->loadIntoField(j, field);
        }

        return true;
    }

    void JsonPropertyContent::readFrom(Field &field) {
        PAX_NOT_IMPLEMENTED();
    }
}
