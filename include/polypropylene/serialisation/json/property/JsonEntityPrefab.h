//
// Created by Bittner on 01/03/2019.
//

#ifndef POLYPROPYLENE_JSONENTITYPREFAB_H
#define POLYPROPYLENE_JSONENTITYPREFAB_H

#include <polypropylene/property/EntityPrefab.h>
#include <polypropylene/serialisation/json/JsonUtil.h>
#include <polypropylene/io/Path.h>

#include <polypropylene/property/Property.h>

#include "polypropylene/serialisation/ContentProvider.h"
#include "JsonPropertyContent.h"

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
            using json = nlohmann::json;

            std::shared_ptr<json> rootNode;
            Path path;

            void parse(json &parent, const std::string &childname, EntityType & e, const VariableRegister & variableRegister) {
                const auto &parserRegister = Parsers.getRegister();
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
            static JsonEntityPrefabElementParserRegister<EntityType> Parsers;

            explicit JsonEntityPrefab(const std::shared_ptr<json> &file, const Path &path)
                    : EntityPrefab<EntityType>(), rootNode(file), path(path) {}

            virtual ~JsonEntityPrefab() = default;

            Path resolvePath(const std::string & str) {
                Path p = Path(VariableResolver::resolveVariables(str, Prefab::PreDefinedVariables));

                if (p.isRelative()) {
                    p = getPath().getDirectory() + p;
                }

                return p;
            }

            static void initialize(Resources &resources) {
                Parsers.registerParser(
                        "Inherits",
                        [&resources](json &node, EntityType &e, JsonEntityPrefab<EntityType> &prefab, const VariableRegister & variableRegister) {
                            for (auto &el : node.items()) {
                                Path parentPath = prefab.path.getDirectory() + el.value();
                                std::shared_ptr<EntityPrefab<EntityType>> parentPrefab;

                                const auto &it = prefab.parentPrefabs.find(parentPath);
                                if (it != prefab.parentPrefabs.end()) {
                                    parentPrefab = it->second;
                                } else {
                                    parentPrefab = resources.loadOrGet<EntityPrefab<EntityType>>(
                                            parentPath);
                                    prefab.parentPrefabs[parentPath] = parentPrefab;
                                }

                                parentPrefab->addMyContentTo(e, variableRegister);
                            }
                        });

                Parsers.registerParser(
                        "Properties",
                        [&resources](json &node, EntityType & e, JsonEntityPrefab<EntityType> &prefab, const VariableRegister & variableRegister) {
                            std::vector<Property<EntityType> *> props;

                            ContentProvider contentProvider(resources,
                                                            variableRegister);

                            for (auto &el : node.items()) {
                                const std::string propTypeName = el.key();
                                IPropertyFactory<EntityType> *propertyFactory = PropertyFactoryRegister<EntityType>::getFactoryFor(
                                        propTypeName);

                                if (propertyFactory) {
                                    JsonPropertyContent content(el.value());
                                    contentProvider.setContent(&content);

                                    // If the entity already has properties of the given type we won't create a new one
                                    // but instead overwrite the old ones with the newer settings.
                                    const PAX::TypeHandle &propType = propertyFactory->getPropertyType();
                                    const bool isPropMultiple = propertyFactory->isPropertyMultiple();

                                    if (!isPropMultiple && e.has(propType, isPropMultiple)) {
                                        Property<EntityType> * property = e.getSingle(propType);
                                        contentProvider.writeToMetadata(property->getMetadata(), ContentProvider::Options::IgnoreMandatoryFlags);
                                    } else {
                                        Property<EntityType> * property = propertyFactory->create(contentProvider);
                                        contentProvider.writeToMetadata(property->getMetadata());
                                        props.emplace_back(property);
                                    }


                                    contentProvider.setContent(nullptr);
                                }
                            }

                            // Add the properties deferred to resolve their dependencies.
                            while (!props.empty()) {
                                size_t numOfPropsToAdd = props.size();

                                for (auto it = props.begin(); it != props.end(); ++it) {
                                    if ((*it)->areDependenciesMetFor(e)) {
                                        e.add(*it);
                                        props.erase(it);
                                        break;
                                    }
                                }

                                if (numOfPropsToAdd == props.size()) {
                                    // Not a single property could be added to the Entity because not a single dependency is met!
                                    PAX_LOG(Log::Level::Error, "Parsing \"Properties\": Error during adding properties! Dependencies could not be met!");
                                    break;
                                }
                            }
                        });
            }

            EntityType * create(const VariableRegister & variableRegister) override {
                EntityType * e = nullptr;

                // TODO: Agree on global Allocator for PropertyContainers!!!
                e = new EntityType();

                addMyContentTo(*e, variableRegister);
                return e;
            }

            void addMyContentTo(EntityType &e, const VariableRegister & variableRegister) override {
                // Compose given variables with the predefined ones.
                // Therefore, copy the given VariableRegister, such that duplicates
                // are override with the custom variables.
                VariableRegister composedVariableRegister = variableRegister;
                composedVariableRegister.insert(EntityPrefab<EntityType>::PreDefinedVariables.begin(),
                                                EntityPrefab<EntityType>::PreDefinedVariables.end());

                std::vector<std::string> parseOrder = {
                        "Inherits",
                        "Properties"
                };

                for (const std::string & name : parseOrder) {
                    if (rootNode->count(name) > 0) {
                        parse(*rootNode.get(), name, e, composedVariableRegister);
                    }
                }

                for (auto &el : rootNode->items()) {
                    if (!Util::vectorContains(parseOrder, el.key())) {
                        parse(*rootNode.get(), el.key(), e, composedVariableRegister);
                    }
                }
            }

            const Path & getPath() {
                return path;
            }
        };

        template<typename EntityType>
        JsonEntityPrefabElementParserRegister<EntityType> JsonEntityPrefab<EntityType>::Parsers;
    }
}

#endif //POLYPROPYLENE_JSONENTITYPREFAB_H
