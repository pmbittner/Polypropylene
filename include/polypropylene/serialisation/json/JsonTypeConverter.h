//
// Created by Bittner on 21/05/2019.
//

#ifndef POLYPROPYLENE_JSONTYPECONVERTER_H
#define POLYPROPYLENE_JSONTYPECONVERTER_H

#ifdef PAX_WITH_JSON

#include "JsonUtil.h"
#include "nlohmann/Json.h"
#include "polypropylene/stdutils/StringUtils.h"

/**
 * Convencience macros for implementing json parsers.
 * Explanations and examples can be found in the project's wiki:
 * https://github.com/pmbittner/Polypropylene/wiki/Parsing-Custom-Types-From-and-to-Json
 */

#define PAX_DECLARE_JSON_CONVERTER_FOR(type) PAX_DECLARE_TYPE_CONVERTER_FOR(::nlohmann::json, type)
#define PAX_IMPLEMENT_JSON_CONVERT_TO(type) PAX_IMPLEMENT_CONVERT_TO(::nlohmann::json, type)
#define PAX_IMPLEMENT_JSON_CONVERT_FROM(type) PAX_IMPLEMENT_CONVERT_FROM(::nlohmann::json, type)
#define PAX_IMPLEMENT_JSON_CONVERT_FROM_WITH_OSTREAM(type) \
    PAX_IMPLEMENT_CONVERT_FROM(::nlohmann::json, type) { \
        std::stringstream s; \
        s << x; \
        return s.str(); \
    }

namespace PAX {
    /**
     * General interface for parsing json objects to custom types.
     * This is done by specializing the more general TypeConverter<From, To> template class.
     * The default implementation reuses the string parser by interpreting the given json as a plain string.
     * @tparam T The type to which the json object should be parsed.
     */
    template<class T>
    class TypeConverter<nlohmann::json, T> {
    public:
        PAX_NODISCARD static T convertTo(nlohmann::json const & j) {
            return TypeConverter<std::string, T>::convertTo(JsonToString(j));
        }

        PAX_NODISCARD static nlohmann::json convertFrom(T const & x) {
            return TypeConverter<std::string, T>::convertFrom(x);
        }
    };

    namespace Json {
        template<typename T>
        PAX_NODISCARD T convertTo(nlohmann::json const &j) {
            return TypeConverter<nlohmann::json, T>::convertTo(j);
        }

        template<typename T>
        PAX_NODISCARD nlohmann::json convertFrom(T const & x) {
            return TypeConverter<nlohmann::json, T>::convertFrom(x);
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

PAX_DECLARE_JSON_CONVERTER_FOR(Path)

#include <polypropylene/serialisation/json/JsonStdContainerConverters.h>

#endif // PAX_WITH_JSON

#endif //POLYPROPYLENE_JSONTYPECONVERTER_H
