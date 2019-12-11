//
// Created by Bittner on 01/03/2019.
//

#ifndef POLYPROPYLENE_JSONPROPERTYCONTAINERPREFABLOADER_H
#define POLYPROPYLENE_JSONPROPERTYCONTAINERPREFABLOADER_H

#include <proteus/resources/ResourceLoader.h>
#include <proteus/io/Path.h>
#include <proteus/io/FileTypeChecker.h>
#include <proteus/resources/Resources.h>

#include "JsonPropertyContainerPrefab.h"

namespace PAX {
    namespace Json {
        template<typename C>
        class JsonPropertyContainerPrefabLoader : public ResourceLoader<PropertyContainerPrefab<C>, Path> {
            Resources &resources;

        public:
            /**
             * As the Prefabs will be loaded from JSON files, a ResourceManager is required, that is able
             * to load JSON files.
             * The <proteus/json/JsonLoader.h> can be used therefore.
             */
            explicit JsonPropertyContainerPrefabLoader(Resources &resources) : resources(resources) {}

            bool canLoad(Path path) const override {
                return Util::FileTypeChecker({"json"}).check(path);
            }

            std::shared_ptr<PropertyContainerPrefab<C>> load(Path path) override {
                std::shared_ptr<nlohmann::json> j = resources.loadOrGet<nlohmann::json>(path);
                if (j) {
                    return std::make_shared<JsonPropertyContainerPrefab<C>>(j, path);
                }
                return nullptr;
            }

            std::shared_ptr<PropertyContainerPrefab<C>>
            loadToOrGetFromResources(Resources &resources, const VariableHierarchy &parameters) override {
                PAX_NOT_IMPLEMENTED_EXCEPTION();
            }
        };
    }
}

#endif //POLYPROPYLENE_JSONPROPERTYCONTAINERPREFABLOADER_H
