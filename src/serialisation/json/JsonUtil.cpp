//
// Created by Paul on 02.07.2019.
//

#include <sstream>
#include <polypropylene/serialisation/json/JsonUtil.h>
#include <polypropylene/serialisation/json/nlohmann/Json.h>

namespace PAX {
    std::string JsonToString(const nlohmann::json & j) {
        if (j.is_string()) {
            return j;
        }

        return j.dump();
    }

    nlohmann::json StringToJson(const std::string & s) {
        bool is_literal = s.find('[') == std::string::npos && s.find('{') == std::string::npos;

        if (is_literal) {
            return s;
        } else {
            return nlohmann::json::parse(s);
        }
    }

    Path JsonToPath(const nlohmann::json & j) {
        const auto & it = j.find("Path");
        if (it != j.end()) {
            return Path(JsonToString(it.value()));
        }

        return Path(JsonToString(j));
    }

    void setJsonValue(nlohmann::json & j, const std::string & key, const std::string & value) {
        j.emplace(key, value);
    }
}