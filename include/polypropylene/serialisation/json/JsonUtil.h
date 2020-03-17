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
        virtual ~JsonElementParser() {};
        virtual void parse(nlohmann::json &, Args...) = 0;
    };

    std::string JsonToString(const nlohmann::json & j);
    Path JsonToPath(const nlohmann::json & j);
    nlohmann::json StringToJson(const std::string & s);
}

#endif //PAX_WITH_JSON

#endif //POLYPROPYLENE_JSONUTIL_H
