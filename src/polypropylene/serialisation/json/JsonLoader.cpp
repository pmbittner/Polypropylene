//
// Created by Bittner on 01/03/2019.
//

#include <polypropylene/serialisation/json/JsonLoader.h>
#include <polypropylene/serialisation/json/Json.h>
#include <polypropylene/io/FileTypeChecker.h>
#include <polypropylene/resources/Resources.h>
#include <fstream>

namespace PAX {
    namespace Json {
        bool JsonLoader::canLoad(PAX::Path path) const {
            Util::FileTypeChecker formats({"JSON"});
            return formats.check(path);
        }

        std::shared_ptr<nlohmann::json> JsonLoader::load(PAX::Path path) {
            try {
                std::ifstream fileStream(path.c_str());
                nlohmann::json j;
                fileStream >> j;
                return std::make_shared<nlohmann::json>(j);
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

        void JsonLoader::write(const nlohmann::json &json, const Path & path) const {
            std::ofstream fileStream(path.c_str());
            fileStream << json;
            fileStream.close();
        }

        std::shared_ptr<nlohmann::json>
        JsonLoader::loadToOrGetFromResources(Resources &resources, const VariableHierarchy &parameters) {
            // Only one entry is required, namely the Path
            if (parameters.values.size() == 1) {
                const std::string &key = parameters.values.begin()->first;
                if (!key.empty()) {
                    return resources.loadOrGet<nlohmann::json>(Path(key));
                }
                const std::string &value = parameters.values.begin()->second;
                if (!value.empty()) {
                    return resources.loadOrGet<nlohmann::json>(Path(value));
                }
            }

            return nullptr;
        }
    }
}