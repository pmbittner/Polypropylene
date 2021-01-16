//
// Created by Bittner on 01/03/2019.
//

#ifndef POLYPROPYLENE_JSONUTIL_H
#define POLYPROPYLENE_JSONUTIL_H

#ifdef PAX_WITH_JSON
#include "polypropylene/serialisation/json/nlohmann/JsonFwd.h"
#include "polypropylene/io/Path.h"

namespace PAX {
    template<typename... Args>
    class JsonElementParser {
    protected:
    public:
        virtual ~JsonElementParser() = default;
        virtual void parse(nlohmann::json &, Args...) = 0;
    };

    std::string JsonToString(const nlohmann::json & j);
    Path JsonToPath(const nlohmann::json & j);
    nlohmann::json StringToJson(const std::string & s);

    /**
     * Helper function to avoid having to include Json.h in header files.
     * Invokes: j.emplace(key, value)
     */
    void setJsonValue(nlohmann::json & j, const std::string & key, const std::string & value);
}

#endif //PAX_WITH_JSON

#endif //POLYPROPYLENE_JSONUTIL_H
