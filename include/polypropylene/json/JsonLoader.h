//
// Created by Bittner on 01/03/2019.
//

#ifndef POLYPROPYLENE_JSONLOADER_H
#define POLYPROPYLENE_JSONLOADER_H

#include <polypropylene/resources/ResourceLoader.h>
#include "JsonFwd.h"

namespace PAX {
    namespace Json {
        class JsonLoader : public ResourceLoader<nlohmann::json, Path> {
        public:
            std::shared_ptr<nlohmann::json> loadToOrGetFromResources(Resources &resources, const VariableHierarchy &parameters) override;
            bool canLoad(Path) const override;
            std::shared_ptr<nlohmann::json> load(Path) override;
        };
    }
}

#endif //POLYPROPYLENE_JSONLOADER_H
