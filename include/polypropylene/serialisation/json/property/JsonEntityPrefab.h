//
// Created by Bittner on 01/03/2019.
//

#ifndef POLYPROPYLENE_JSONENTITYPREFAB_H
#define POLYPROPYLENE_JSONENTITYPREFAB_H

#include <polypropylene/property/EntityPrefab.h>
#include <polypropylene/serialisation/json/JsonUtil.h>
#include <polypropylene/io/Path.h>

#include <polypropylene/property/Property.h>
#include <polypropylene/serialisation/json/JsonParser.h>
#include <polypropylene/stdutils/CollectionUtils.h>

#include "polypropylene/serialisation/ClassMetadataSerialiser.h"
#include "polypropylene/serialisation/json/JsonFieldStorage.h"

namespace PAX {
    namespace Json {
        template<typename EntityType>
        class JsonEntityPrefab;

        template<typename EntityType>
        using JsonEntityPrefabElementParser = JsonElementParser<EntityType &, JsonEntityPrefab<EntityType> &, const VariableRegister &>;

        template<typename EntityType>
        class LambdaJsonEntityPrefabElementParser : public JsonEntityPrefabElementParser<EntityType> {
        public:
            using Callback = std::function<void(nlohmann::json &, EntityType &, JsonEntityPrefab<EntityType> &, const VariableRegister &)>;

        private:
            Callback callback;

        public:
            explicit LambdaJsonEntityPrefabElementParser(const Callback & function)
                    : JsonEntityPrefabElementParser<EntityType>(), callback(function) {}

            ~LambdaJsonEntityPrefabElementParser() override = default;

            void parse(nlohmann::json &j, EntityType &c, JsonEntityPrefab<EntityType> &prefab, const VariableRegister & variableRegister) override {
                callback(j, c, prefab, variableRegister);
            }
        };

        template<typename EntityType>
        class JsonEntityPrefabElementParserRegister {
            std::map<std::string, JsonEntityPrefabElementParser<EntityType> *> parsers;

        public:
            JsonEntityPrefabElementParserRegister() = default;

            bool registerParser(const std::string &name, JsonEntityPrefabElementParser<EntityType> *parser) {
                const auto &it = parsers.find(name);
                if (it == parsers.end()) {
                    parsers[name] = parser;
                    return true;
                }

                return false;
            }

            bool registerParser(const std::string &name,
                                const typename LambdaJsonEntityPrefabElementParser<EntityType>::Callback &lambda) {
                return registerParser(name, new LambdaJsonEntityPrefabElementParser<EntityType>(lambda));
            }

            bool unregisterParser(const std::string &name) {
                const auto &it = parsers.find(name);
                if (it != parsers.end()) {
                    parsers.erase(it);
                    return true;
                }

                return false;
            }

            const std::map<std::string, JsonEntityPrefabElementParser<EntityType> *> & getRegister() {
                return parsers;
            }
        };

        template<typename EntityType>
        class JsonEntityPrefab : public EntityPrefab<EntityType> {
            struct DefaultElements {
                static constexpr const char * Inherits = "Inherits";
                static constexpr const char * Properties = "Properties";
            };

            using json = nlohmann::json;

            static JsonParserRegister * GlobalParsers;

            std::vector<JsonEntityPrefab<EntityType>> parentPrefabs;
            json rootNode;
            Path path;

            void parse(json &parent, const std::string &childname, EntityType & e, const VariableRegister & variableRegister) {
                const auto &parserRegister = ElementParsers.getRegister();
                const auto &it = parserRegister.find(childname);
                if (it != parserRegister.end()) {
                    if (parent.count(childname) > 0) {
                        it->second->parse(parent[childname], e, *this, variableRegister);
                    }
                } else {
                    PAX_LOG(Log::Level::Warn, "ignoring element " << childname << " because no parser is registered for it!");
                }
            }

        public:
            static JsonEntityPrefabElementParserRegister<EntityType> ElementParsers;

            explicit JsonEntityPrefab(const json & j, const Path & path)
                    : EntityPrefab<EntityType>(), rootNode(j), path(path) {}

            explicit JsonEntityPrefab(EntityPrefab<EntityType> & other) {
                json & propertiesNode = rootNode[DefaultElements::Properties];

                EntityType * e = other.create({});
                const std::vector<Property<EntityType>*> & properties = e->getAllProperties();

                ClassMetadataSerialiser serialiser({});
                for (Property<EntityType> * property : properties) {
                    IPropertyFactory<EntityType> * factory = PropertyFactoryRegister<EntityType>::getFactoryFor(property->getClassType().id);
                    json & propertyNode = propertiesNode[factory->getPropertyName()];
                    JsonFieldStorage storage(propertyNode, *GlobalParsers);
                    serialiser.setStorage(&storage);
                    serialiser.readFromMetadata(property->getMetadata());
                    serialiser.setStorage(nullptr);
                }

                delete e;
            }

            virtual ~JsonEntityPrefab() = default;

            Path resolvePath(const std::string & str) {
                Path p = Path(VariableResolver::resolveVariables(str, Prefab::PreDefinedVariables));

                if (p.isRelative()) {
                    p = getPath().getDirectory() + p;
                }

                return p;
            }

            static void initialize(JsonParserRegister & jsonParserRegister);

            void addMyContentTo(EntityType &e, const VariableRegister & variableRegister) override {
                // Compose given variables with the predefined ones.
                // Therefore, copy the given VariableRegister, such that duplicates
                // are overriden with the custom variables.
                // TODO: Check if this is the correct order.
                VariableRegister composedVariableRegister = variableRegister;
                composedVariableRegister.insert(EntityPrefab<EntityType>::PreDefinedVariables.begin(),
                                                EntityPrefab<EntityType>::PreDefinedVariables.end());

                std::vector<std::string> parseOrder = {
                        DefaultElements::Inherits,
                        DefaultElements::Properties
                };

                for (const std::string & name : parseOrder) {
                    if (rootNode.count(name) > 0) {
                        parse(rootNode, name, e, composedVariableRegister);
                    }
                }

                for (auto &el : rootNode.items()) {
                    if (!Util::vectorContains(parseOrder, el.key())) {
                        parse(rootNode, el.key(), e, composedVariableRegister);
                    }
                }
            }

            PAX_NODISCARD nlohmann::json toJson() {
                json me;
                json & propertiesNode = me[DefaultElements::Properties];

                EntityType * e = EntityPrefab<EntityType>::create({});
                ClassMetadataSerialiser serialiser({});
                const std::vector<Property<EntityType>*> & properties = e->getAllProperties();
                for (Property<EntityType> * p : properties) {
                    IPropertyFactory<EntityType> * factory = PropertyFactoryRegister<EntityType>::getFactoryFor(p->getClassType().id);
                    json & propertyNode = propertiesNode[factory->getPropertyName()];
                    JsonFieldStorage storage(propertyNode, *GlobalParsers);
                    serialiser.setStorage(&storage);
                    ClassMetadata m = p->getMetadata();
                    serialiser.readFromMetadata(m);
                }
                delete e;
                return me;
            }

            PAX_NODISCARD const Path & getPath() const {
                return path;
            }
        };

        template<typename EntityType>
        JsonParserRegister * JsonEntityPrefab<EntityType>::GlobalParsers = nullptr;

        template<typename EntityType>
        JsonEntityPrefabElementParserRegister<EntityType> JsonEntityPrefab<EntityType>::ElementParsers;
    }
}

#define POLYPROPYLENE_JSONENTITYPREFAB_DEFINED
#include "JsonEntityPrefabLoader.h"
#include "JsonEntityPrefabImpl.h"

#endif //POLYPROPYLENE_JSONENTITYPREFAB_H
