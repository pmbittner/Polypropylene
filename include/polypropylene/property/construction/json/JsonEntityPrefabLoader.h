//
// Created by Bittner on 01/03/2019.
//

#ifndef POLYPROPYLENE_JSONENTITYPREFABLOADER_H
#define POLYPROPYLENE_JSONENTITYPREFABLOADER_H

#include <polypropylene/resources/ResourceLoader.h>
#include <polypropylene/io/Path.h>
#include <polypropylene/io/FileTypeChecker.h>
#include <polypropylene/resources/Resources.h>

#include "JsonEntityPrefab.h"

namespace PAX {
    namespace Json {
        template<typename EntityType>
        class JsonEntityPrefabLoader : public ResourceLoader<EntityPrefab<EntityType>, Path> {
            Resources &resources;

        public:
            /**
             * As the Prefabs will be loaded from JSON files, a ResourceManager is required, that is able
             * to load JSON files.
             * The <polypropylene/json/JsonLoader.h> can be used therefore.
             */
            explicit JsonEntityPrefabLoader(Resources &resources) : resources(resources) {}

            PAX_NODISCARD bool canLoad(Path path) const override {
                return Util::FileTypeChecker({"json"}).check(path);
            }

            std::shared_ptr<EntityPrefab<EntityType>> load(Path path) override {
                std::shared_ptr<nlohmann::json> j = resources.loadOrGet<nlohmann::json>(path);
                if (j) {
                    return std::make_shared<JsonEntityPrefab<EntityType>>(j, path);
                }
                return nullptr;
            }

            std::shared_ptr<EntityPrefab<EntityType>>
            loadToOrGetFromResources(Resources &resources, const VariableHierarchy &parameters) override {
                PAX_NOT_IMPLEMENTED_EXCEPTION();
            }
        };
    }
}

#endif //POLYPROPYLENE_JSONENTITYPREFABLOADER_H
