//
// Created by Bittner on 01/03/2019.
//

#ifndef POLYPROPYLENE_JSONENTITYPREFABLOADER_H
#define POLYPROPYLENE_JSONENTITYPREFABLOADER_H

#ifdef PAX_WITH_JSON

#include <polypropylene/io/Path.h>
#include "polypropylene/serialisation/json/nlohmann/JsonFwd.h"

namespace PAX::Json {
    template<typename EntityType>
    class JsonEntityPrefab;

    template<typename EntityType>
    class JsonEntityPrefabLoader {
    public:
        /**
         * As the Prefabs will be loaded from JSON files, a ResourceManager is required, that is able
         * to load JSON files.
         * The <polypropylene/json/JsonLoader.h> can be used therefore.
         */
        explicit JsonEntityPrefabLoader() = default;

        /**
         * Loads the prefab specified in the json file at the given path.
         * @param path Path to a json file containing a description for a prefab.
         * @return The prefab from the json file.
         */
        PAX_NODISCARD JsonEntityPrefab<EntityType> load(const Path & path) const;

        /**
         * Loads the prefab from the given json object.
         * The path is required because json files may contain import directives to other json files whose location
         * has to be resolved.
         * @param json The json object containing the description of the prefab.
         * @param path Path of the json object on disk that is necessary to resolve import paths. If you are sure that
         * the given json object does not contain any file imports, the path can be empty.
         * @return The prefab constructed from the given json instance.
         */
        PAX_NODISCARD JsonEntityPrefab<EntityType> load(const nlohmann::json & json, const Path & path) const;

        void write(JsonEntityPrefab<EntityType> & prefab, const Path & path);
    };
}

#define POLYPROPYLENE_JSONENTITYPREFABLOADER_DEFINED
#include "JsonEntityPrefab.h"
#include "JsonEntityPrefabLoaderImpl.h"

#endif // PAX_WITH_JSON

#endif //POLYPROPYLENE_JSONENTITYPREFABLOADER_H
