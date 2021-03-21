//
// Created by Paul Bittner on 20.03.2021.
//

#ifndef POLYPROPYLENE_JSONSTDCONTAINERCONVERTERS_H
#define POLYPROPYLENE_JSONSTDCONTAINERCONVERTERS_H

#include "nlohmann/Json.h"
#include "JsonTypeConverter.h"

namespace PAX {
    /**
     * Default implementation for parsing maps.
     * Keys and Values are parsed with PAX::Json::convertTo<...>
     */
    template<typename K, typename V>
    class TypeConverter<nlohmann::json, std::map<K, V>> {
    public:
        PAX_NODISCARD static std::map<K, V> convertTo(const nlohmann::json & j) {
            std::map<K, V> m;
            for (const auto & entry : j.items()) {
                const K key = PAX::String::convertTo<K>(entry.key());
                m[key] = PAX::Json::convertTo<V>(entry.value());
            }
            return m;
        }

        PAX_NODISCARD static nlohmann::json convertFrom(const std::map<K, V> & m) {
            nlohmann::json node;
            for (const auto & entry : m) {
                const std::string key = String::convertFrom<K>(entry.first);
                node[key] = Json::convertFrom<V>(entry.second);
            }
            return node;
        }
    };
}

#endif //POLYPROPYLENE_JSONSTDCONTAINERCONVERTERS_H
