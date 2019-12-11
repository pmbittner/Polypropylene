//
// Created by Bittner on 01/03/2019.
//

#ifndef POLYPROPYLENE_JSONUTIL_H
#define POLYPROPYLENE_JSONUTIL_H

#include "JsonFwd.h"

namespace PAX {
    template<typename... Args>
    class JsonElementParser {
    protected:
    public:
        virtual ~JsonElementParser() {};
        virtual void parse(nlohmann::json &, Args...) = 0;
    };

    std::string JsonToString(const nlohmann::json & j);
    nlohmann::json StringToJson(const std::string & s);
}

#endif //POLYPROPYLENE_JSONUTIL_H
