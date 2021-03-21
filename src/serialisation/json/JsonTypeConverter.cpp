//
// Created by Bittner on 20.12.2019.
//

#include "polypropylene/serialisation/json/JsonTypeConverter.h"
#include "polypropylene/serialisation/json/nlohmann/Json.h"

namespace PAX {
    PAX_IMPLEMENT_JSON_CONVERT_TO(Path) { return JsonToPath(x); }
    PAX_IMPLEMENT_JSON_CONVERT_FROM(Path) { return Path::toAbsolute(x); }

    namespace Json {
        bool assertPresence(const nlohmann::json & j, const std::string & childName, bool crashOnError) {
            if (j.count(childName) > 0) return true;

            if (crashOnError) {
                PAX_THROW_RUNTIME_ERROR("Missing variable \"" << childName << "\" in " << JsonToString(j) << "!");
            } else {
                return false;
            }
        }
    }
}