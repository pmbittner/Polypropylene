//
// Created by Bittner on 01/03/2019.
//

#include <fstream>
#include <iomanip>

#include "polypropylene/serialisation/json/JsonLoader.h"
#include "polypropylene/serialisation/json/nlohmann/Json.h"

namespace PAX::Json {
    nlohmann::json JsonLoader::load(const PAX::Path & path) const {
        try {
            std::ifstream fileStream(path.c_str());
            nlohmann::json j;
            fileStream >> j;
            return j;
        }
        catch (nlohmann::json::parse_error &e) {
            PAX_LOG(Log::Level::Error, "File at path "
                    << path
                    << " could not be parsed:\n"
                    << e.what() << '\n'
                    << "exception id: " << e.id << '\n'
                    << "byte position of error: " << e.byte);
            return nullptr;
        }
    }

    void JsonLoader::write(const nlohmann::json &json, const Path & path, unsigned short indentLength) const {
        std::ofstream fileStream(path.c_str());
        fileStream << std::setw(indentLength) << json;
        fileStream.close();
    }
}