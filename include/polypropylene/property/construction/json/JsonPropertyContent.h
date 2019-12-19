//
// Created by Paul on 02.03.2019.
//

#ifndef POLYPROPYLENE_JSONPROPERTYCONTENT_H
#define POLYPROPYLENE_JSONPROPERTYCONTENT_H

#ifdef PAX_WITH_JSON

#include <polypropylene/property/construction/ContentProvider.h>
#include <polypropylene/json/Json.h>

namespace PAX {
    namespace Json {
        class JsonPropertyContent : public Internal::PropertyContent {
            nlohmann::json node;

        protected:
            std::string getValue(const std::string &key, const VariableRegister & variables) override;
            std::vector<std::string> getValues(const std::string & key, const VariableRegister & variables) override;

        public:
            explicit JsonPropertyContent(const nlohmann::json &node);
            ~JsonPropertyContent() override;

            bool has(const std::string &name) override;
            VariableHierarchy getResourceParametersFor(const std::string &name) override;
        };
    }
}

#endif // PAX_WITH_JSON

#endif //POLYPROPYLENE_JSONPROPERTYCONTENT_H