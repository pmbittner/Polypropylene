//
// Created by Paul on 02.03.2019.
//

#ifndef POLYPROPYLENE_JSONPROPERTYCONTENT_H
#define POLYPROPYLENE_JSONPROPERTYCONTENT_H

#ifdef PAX_WITH_JSON

#include <polypropylene/serialisation/ContentProvider.h>
// TODO: Find a way to include JsonFwd.h here instead of Json.h
#include <polypropylene/serialisation/json/Json.h>

namespace PAX::Json {
    class JsonPropertyContent : public PropertyContent {
        nlohmann::json node;

    public:
        explicit JsonPropertyContent(const nlohmann::json &node);
        ~JsonPropertyContent() override;

        PAX_NODISCARD std::string getValue(const std::string &key, const VariableRegister & variables) const;

        PAX_NODISCARD bool has(const std::string &name) const override;
        PAX_NODISCARD VariableHierarchy getResourceParametersFor(const std::string &name) const override;
        PAX_NODISCARD bool writeTo(Field & field, const VariableRegister &variables) const override;
        void readFrom(Field & field) override;
    };
}

#endif // PAX_WITH_JSON

#endif //POLYPROPYLENE_JSONPROPERTYCONTENT_H
