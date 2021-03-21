//
// Created by Bittner on 25.12.2019.
//

#include <polypropylene/log/Assert.h>

#if defined(POLYPROPYLENE_JSONENTITYPREFABLOADER_DEFINED) && defined(POLYPROPYLENE_JSONENTITYPREFAB_DEFINED)

#ifndef POLYPROPYLENE_JSONENTITYPREFABIMPL_H
#define POLYPROPYLENE_JSONENTITYPREFABIMPL_H

namespace PAX::Json {
    template<typename EntityType>
    void JsonEntityPrefab<EntityType>::initialize() {
        ParseOrder = {
                DefaultElements::Inherits,
                DefaultElements::Properties
        };

        ElementParsers.registerParser(
                DefaultElements::Inherits,
                [](json &node, EntityType &e, JsonEntityPrefab<EntityType> &prefab, const VariableRegister & variableRegister) {
                    JsonEntityPrefabLoader<EntityType> prefabLoader;

                    for (auto &el : node.items()) {
                        Path parentPath = prefab.path.getDirectory() + el.value();
                        JsonEntityPrefab<EntityType> * parentPrefab = nullptr;

                        for (JsonEntityPrefab<EntityType> & parent : prefab.parentPrefabs) {
                            if (parent.getPath() == parentPath) {
                                parentPrefab = &parent;
                                break;
                            }
                        }

                        if (!parentPrefab) {
                            prefab.parentPrefabs.emplace_back(prefabLoader.load(parentPath));
                            parentPrefab = &prefab.parentPrefabs.back();
                        }

                        parentPrefab->addMyContentTo(e, variableRegister);
                    }
                });

        ElementParsers.registerParser(
                DefaultElements::Properties,
                [](json &node, EntityType & e, const JsonEntityPrefab<EntityType> &prefab, const VariableRegister & variableRegister) {
                    if (!node.is_array()) {
                        PAX_THROW_RUNTIME_ERROR("Expected array but was given " << std::endl << node.dump(2));
                    }
                    std::vector<PropertyType *> props;
                    ClassMetadataSerialiser serialiser(variableRegister);

                    // For each proprety that is defined in the json array
                    for (auto &propertyNode : node) {
                        std::string propTypeName = "none";
                        for (auto propertyNodeEntries : propertyNode.items()) {
                            if (propTypeName == "none") {
                                propTypeName = propertyNodeEntries.key();
                            } else {
                                PAX_THROW_RUNTIME_ERROR("Expected only one property but got multiple in:" << std::endl
                                                                                                          << propertyNode.dump(
                                                                                                                  2));
                            }
                        }

                        if (propTypeName == "none") {
                            PAX_THROW_RUNTIME_ERROR("Expected property in blank object" << std::endl << propertyNode.dump(2) << std::endl <<" in:" << std::endl << node.dump(2));
                        }
                        IPropertyFactory<EntityType> *propertyFactory = PropertyFactoryRegister<EntityType>::getFactoryFor(
                                propTypeName);
                        const PAX::Type &propType = propertyFactory->getPropertyType();
                        const bool isPropMultiple = propertyFactory->isPropertyMultiple();

                        PropertyType * property = nullptr;
                        ClassMetadataSerialiser::Options options = ClassMetadataSerialiser::Options::None;

                        // If the entity already has properties of the given type we won't create a new one
                        // but instead overwrite the old ones with the newer settings.
                        if (!isPropMultiple && e.has(propType.id, isPropMultiple)) {
                            property = e.getSingle(propType.id);
                            options = ClassMetadataSerialiser::Options::IgnoreMandatoryFlags;
                        } else {
                            property = propertyFactory->create();
                            props.emplace_back(property);
                        }

                        // If there are any fields given, set them in the property.
                        json & fields = propertyNode[propTypeName];
                        if (!fields.empty()) {
                            JsonFieldStorage storage(fields, JsonFieldWriterRegister::Instance());
                            serialiser.setStorage(&storage);
                            ClassMetadata metadata = property->getMetadata();
                            serialiser.writeToMetadata(metadata, options);
                            serialiser.setStorage(nullptr);
                        }
                    }

                    // Add the properties deferred to resolve their dependencies.
                    while (!props.empty()) {
                        size_t numOfPropsToAdd = props.size();

                        for (auto it = props.begin(); it != props.end(); ++it) {
                            if ((*it)->areDependenciesMetFor(e)) {
                                (*it)->created();
                                e.add(*it);
                                props.erase(it);
                                break;
                            }
                        }

                        if (numOfPropsToAdd == props.size()) {
                            // Not a single property could be added to the Entity because not a single dependency is met!
                            std::stringstream errorMessageStream;
                            errorMessageStream << "Parsing \"Properties\": Error during adding properties! Dependencies could not be met for properties:\n" << std::endl;

                            for (auto it = props.begin(); it != props.end(); ++it) {
                                errorMessageStream << "  " << (*it)->getClassType().name() << std::endl;
                            }

                            PAX_LOG(Log::Level::Error, errorMessageStream.str());
                            break;
                        }
                    }
                });
    }
}

#endif //POLYPROPYLENE_JSONENTITYPREFABIMPL_H

#endif