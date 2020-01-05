//
// Created by Bittner on 25.12.2019.
//

#if defined(POLYPROPYLENE_JSONENTITYPREFABLOADER_DEFINED) && defined(POLYPROPYLENE_JSONENTITYPREFAB_DEFINED)

#ifndef POLYPROPYLENE_JSONENTITYPREFABLOADERIMPL_H
#define POLYPROPYLENE_JSONENTITYPREFABLOADERIMPL_H

#include "JsonEntityPrefabLoader.h"
#include "../JsonLoader.h"
#include "../Json.h"

namespace PAX::Json {
    template<typename EntityType>
    PAX_NODISCARD JsonEntityPrefab<EntityType> JsonEntityPrefabLoader<EntityType>::load(const Path & path) const {
        JsonLoader jsonLoader;
        nlohmann::json j = jsonLoader.load(path);
        return load(j, path);
    }

    template<typename EntityType>
    PAX_NODISCARD JsonEntityPrefab<EntityType> JsonEntityPrefabLoader<EntityType>::load(const nlohmann::json & json, const Path & path) const {
        return JsonEntityPrefab<EntityType>(json, path);
    }

    template<typename EntityType>
    void JsonEntityPrefabLoader<EntityType>::write(JsonEntityPrefab<EntityType> & prefab, const Path & path) {
        JsonLoader loader;
        nlohmann::json j = prefab.toJson();
        loader.write(j, path);
    }
}

#endif //POLYPROPYLENE_JSONENTITYPREFABLOADERIMPL_H

#endif