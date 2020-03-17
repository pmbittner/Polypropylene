//
// Created by Bittner on 21/05/2019.
//

#ifndef POLYPROPYLENE_JSONPARSER_H
#define POLYPROPYLENE_JSONPARSER_H

#ifdef PAX_WITH_JSON

#include "JsonUtil.h"
#include "polypropylene/stdutils/StringUtils.h"

// TODO: Try to avoid this
#include "polypropylene/serialisation/json/nlohmann/Json.h"

namespace PAX {
    template<class T>
    class TryParser<nlohmann::json, T> {
    public:
        PAX_NODISCARD static T tryParse(const nlohmann::json & j) {
            return ::PAX::String::tryParse<T>(JsonToString(j));
        }
    };

    template<>
    class TryParser<nlohmann::json, Path> {
    public:
        PAX_NODISCARD static Path tryParse(const nlohmann::json & j);
    };

    namespace Json {
        template<typename T>
        PAX_NODISCARD T tryParse(const nlohmann::json &j) {
            return TryParser<nlohmann::json, T>::tryParse(j);
        }

        bool assertPresence(const nlohmann::json & j, const std::string & childName, bool crashOnError = true);
    }
}

#endif // PAX_WITH_JSON

#endif //POLYPROPYLENE_JSONPARSER_H
