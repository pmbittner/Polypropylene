//
// Created by Bittner on 21/05/2019.
//

#ifndef POLYPROPYLENE_JSONPARSER_H
#define POLYPROPYLENE_JSONPARSER_H

#ifdef PAX_WITH_JSON

#include "JsonUtil.h"
#include "polypropylene/stdutils/StringUtils.h"

/**
 * Convencience macros for implementing json parsers.
 * Explanations and examples can be found in the project's wiki:
 * https://github.com/pmbittner/Polypropylene/wiki/Parsing-Custom-Types-From-and-to-Json
 */

#define PAX_DECLARE_JSONPARSER_FOR(type) \
namespace PAX { \
    template<> \
    class TryParser<nlohmann::json, type> { \
    public: \
        PAX_NODISCARD static type tryParse(const nlohmann::json &); \
    }; \
}

#define PAX_IMPLEMENT_JSONPARSER_FOR(type) \
    type PAX::TryParser<nlohmann::json, type>::tryParse(const nlohmann::json & json)


namespace PAX {
    /**
     * General interface for parsing json objects to custom types.
     * This is done by specializing the more general TryParser<From, To> template class.
     * The default implementation reuses the string parser by interpreting the given json as a plain string.
     * @tparam T The type to which the json object should be parsed.
     */
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

        /**
         * Utility method for asserting the presence of a field in a json object.
         * @param j The json object that should contain a field with the given name.
         * @param childName The name of the field (child object) that should be present in the given json object.
         * @param crashOnError Throws a runtime error if this is set to true
         *                     and the given json object does not have a child with the given name.
         * @return True, iff `j` has a field with name `childName`.
         */
        bool assertPresence(const nlohmann::json & j, const std::string & childName, bool crashOnError = true);
    }
}

#endif // PAX_WITH_JSON

#endif //POLYPROPYLENE_JSONPARSER_H
