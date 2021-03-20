//
// Created by Paul Bittner on 20.03.2021.
//

#ifndef POLYPROPYLENE_JSONDEFAULTPARSERS_H
#define POLYPROPYLENE_JSONDEFAULTPARSERS_H

#include "nlohmann/Json.h"
#include "JsonParser.h"

namespace PAX {
    /**
     * Default implementation for parsing maps.
     * Keys and Values are parsed with PAX::Json::tryParse<...>
     */
    template<typename K, typename V>
    class TryParser<nlohmann::json, std::map<K, V>> {
    public:
        PAX_NODISCARD static std::map<K, V> tryParse(const nlohmann::json & j) {
            std::map<K, V> m;
            for (const auto & entry : j.items()) {
                const K key = PAX::String::tryParse<K>(entry.key());
                m[key] = PAX::Json::tryParse<V>(entry.value());
            }
            return m;
        }
    };
}

template<typename K, typename V>
std::ostream & operator<<(std::ostream & str, const std::map<K, V> & m) {
    str << "{";
    size_t i = 0;
    const size_t size = m.size();
    for (const auto & entry : m) {
        str << "\"" << entry.first << "\": " << entry.second;
        if (i < (size - 1)) {
            str << "," << std::endl;
        }
        ++i;
    }
    return str << "}";
}

#endif //POLYPROPYLENE_JSONDEFAULTPARSERS_H
