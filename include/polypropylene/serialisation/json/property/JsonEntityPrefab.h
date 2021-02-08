//
// Created by Bittner on 01/03/2019.
//

#ifndef POLYPROPYLENE_JSONENTITYPREFAB_H
#define POLYPROPYLENE_JSONENTITYPREFAB_H

#include "polypropylene/prefab/Prefab.h"
#include "polypropylene/property/Property.h"

#include "polypropylene/io/Path.h"

#include "polypropylene/serialisation/ClassMetadataSerialiser.h"
#include "polypropylene/serialisation/json/nlohmann/Json.h"
#include "polypropylene/serialisation/json/JsonUtil.h"
#include "polypropylene/serialisation/json/JsonFieldWriter.h"
#include "polypropylene/serialisation/json/JsonFieldStorage.h"

#include "polypropylene/stdutils/CollectionUtils.h"

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

            PAX_MAYBEUNUSED bool unregisterParser(const std::string &name) {
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
        class JsonEntityPrefab : public Prefab<EntityType> {
            struct DefaultElements {
                static constexpr const char * Inherits = "Inherits";
                static constexpr const char * Properties = "Properties";
            };

            using json = nlohmann::json;
            using PropertyType = typename EntityType::PropertyType;

            static JsonFieldWriterRegister * GlobalWriters;

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

            static json ToJson(Prefab<EntityType> & prefab) {
                json ret;
                json propertiesNode;

                EntityType * e = prefab.create({});
                const std::vector<PropertyType*> & properties = e->getAllProperties();

                ClassMetadataSerialiser serialiser({});
                for (PropertyType * property : properties) {
                    IPropertyFactory<EntityType> * factory = PropertyFactoryRegister<EntityType>::getFactoryFor(property->getClassType().type.id);
                    json propertyNode;
                    json & fields = propertyNode[factory->getPropertyName()];
                    JsonFieldStorage storage(fields, *GlobalWriters);
                    serialiser.setStorage(&storage);
                    serialiser.readFromMetadata(property->getMetadata());
                    serialiser.setStorage(nullptr);
                    propertiesNode.push_back(propertyNode);
                }

                ret[DefaultElements::Properties] = propertiesNode;

                pax_delete(e);
                return ret;
            }

        public:
            static JsonEntityPrefabElementParserRegister<EntityType> ElementParsers;
            static std::vector<std::string> ParseOrder;

            /**
             * Creates a JsonEntityPrefab from a json object that was loaded from the
             * given path.
             * @param j The json object that represents a prefab.
             * @param path The path from which the given json was loaded.
             *             This path is used to resolve relative paths in the json.
             */
            explicit JsonEntityPrefab(const json & j, const Path & path)
                    : Prefab<EntityType>(), rootNode(j), path(path) {}

            /**
             * Creates a JsonEntityPrefab from the given Prefab.
             * This allows converting and exporting the given Prefab as json.
             * @param other The prefab to copy.
             * @return A JsonEntityPrefab that is a copy of the given prefab.
             */
            static JsonEntityPrefab<EntityType> FromPrefab(Prefab<EntityType> & other) {
                if (auto * otherAsJson = dynamic_cast<JsonEntityPrefab<EntityType>*>(&other)) {
                    return JsonEntityPrefab(otherAsJson->toJson(), otherAsJson->getPath());
                } else {
                    return JsonEntityPrefab(ToJson(other), Path::EmptyPath);
                }
            }

            virtual ~JsonEntityPrefab() = default;

            /**
             * Given a path as string:
             *   - resolves any variables used in that path (${...}) with the variables in IPrefab::PreDefinedVariables,
             *   - if the path is relative, makes it a path starting from this prefabs path.
             *  For example the path of this JsonEntityPrefab is "/usr/asd"
             *  and IPrefab::PreDefinedVariables["fooDir"] = "baz" we get:
             *    resolvePath("../${fooDir}/bar.txt") = "/usr/baz/bar.txt"
             * @param str The path to resolve variables for and directory if it is relative.
             * @return A path with variables resolved and directory if it is relative.
             */
            PAX_MAYBEUNUSED Path resolvePath(const std::string & str) {
                Path p = Path(VariableResolver::resolveVariables(str, IPrefab::PreDefinedVariables));

                if (p.isRelative()) {
                    p = getPath().getDirectory() + p;
                }

                return p;
            }

            /**
             * Has to be invoked once during program startup.
             * Memorizes the JsonFieldWriters you use.
             */
            static void initialize(JsonFieldWriterRegister & jsonFieldWriterRegister);

            void addMyContentTo(EntityType &e, const VariableRegister & variableRegister) override {
                // Compose given variables with the predefined ones.
                // Therefore, copy the given VariableRegister, such that duplicates
                // are overriden with the custom variables.
                VariableRegister composedVariableRegister = ComposeVariableRegisters(IPrefab::PreDefinedVariables, variableRegister);

                for (const std::string & name : ParseOrder) {
                    if (rootNode.count(name) > 0) {
                        parse(rootNode, name, e, composedVariableRegister);
                    }
                }

                for (auto &el : rootNode.items()) {
                    if (!Util::vectorContains(ParseOrder, el.key())) {
                        parse(rootNode, el.key(), e, composedVariableRegister);
                    }
                }
            }

            PAX_NODISCARD nlohmann::json toJson() {
                if (parentPrefabs.empty()) {
                    return rootNode;
                } else {
                    // If we have parent prefabs, inline their contents.
                    return ToJson(*this);
                }
            }

            PAX_NODISCARD const Path & getPath() const {
                return path;
            }
        };

        template<typename EntityType>
        PAX_MAYBEUNUSED JsonFieldWriterRegister * JsonEntityPrefab<EntityType>::GlobalWriters = nullptr;

        template<typename EntityType>
        PAX_MAYBEUNUSED std::vector<std::string> JsonEntityPrefab<EntityType>::ParseOrder;

        template<typename EntityType>
        PAX_MAYBEUNUSED JsonEntityPrefabElementParserRegister<EntityType> JsonEntityPrefab<EntityType>::ElementParsers;
    }
}

#define POLYPROPYLENE_JSONENTITYPREFAB_DEFINED
#include "JsonEntityPrefabLoader.h"
#include "JsonEntityPrefabImpl.h"

#endif //POLYPROPYLENE_JSONENTITYPREFAB_H
