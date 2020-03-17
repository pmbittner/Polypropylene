//
// Created by Bittner on 20.12.2019.
//

#include <polypropylene/serialisation/json/JsonParser.h>
#include <polypropylene/log/Errors.h>

namespace PAX {
    Path TryParser<nlohmann::json, Path>::tryParse(const nlohmann::json &f) {
        return JsonToPath(f);
    }

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