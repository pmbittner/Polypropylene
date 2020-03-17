//
// Created by Bittner on 01/03/2019.
//

#ifndef POLYPROPYLENE_JSONLOADER_H
#define POLYPROPYLENE_JSONLOADER_H

#ifdef PAX_WITH_JSON

#include <polypropylene/io/Path.h>
#include "polypropylene/serialisation/json/nlohmann/JsonFwd.h"

namespace PAX::Json {
    /**
     * Loads and writes json files from and to disk.
     */
    class JsonLoader {
    public:
        PAX_NODISCARD nlohmann::json load(const Path & path) const;
        void write(const nlohmann::json & json, const Path & path, unsigned short indentLength = 2) const;
    };
}

#endif // PAX_WITH_JSON

#endif //POLYPROPYLENE_JSONLOADER_H
