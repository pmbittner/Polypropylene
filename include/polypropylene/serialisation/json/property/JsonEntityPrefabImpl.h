//
// Created by Bittner on 25.12.2019.
//

#if defined(POLYPROPYLENE_JSONENTITYPREFABLOADER_DEFINED) && defined(POLYPROPYLENE_JSONENTITYPREFAB_DEFINED)

#ifndef POLYPROPYLENE_JSONENTITYPREFABIMPL_H
#define POLYPROPYLENE_JSONENTITYPREFABIMPL_H

namespace PAX::Json {
    template<typename EntityType>
    void JsonEntityPrefab<EntityType>::initialize(JsonFieldWriterRegister * jsonFieldWriterRegister) {
        GlobalWriters = jsonFieldWriterRegister;

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
                [&jsonFieldWriterRegister](json &node, EntityType & e, const JsonEntityPrefab<EntityType> &prefab, const VariableRegister & variableRegister) {
                    std::vector<PropertyType *> props;

                    ClassMetadataSerialiser serialiser(variableRegister);

                    for (auto &el : node.items()) {
                        const std::string & propTypeName = el.key();
                        IPropertyFactory<EntityType> *propertyFactory = PropertyFactoryRegister<EntityType>::getFactoryFor(
                                propTypeName);

                        JsonFieldStorage storage(el.value(), *jsonFieldWriterRegister);
                        serialiser.setStorage(&storage);

                        // If the entity already has properties of the given type we won't create a new one
                        // but instead overwrite the old ones with the newer settings.
                        const PAX::TypeHandle &propType = propertyFactory->getPropertyType();
                        const bool isPropMultiple = propertyFactory->isPropertyMultiple();

                        PropertyType * property = nullptr;
                        ClassMetadataSerialiser::Options options = ClassMetadataSerialiser::Options::None;

                        if (!isPropMultiple && e.has(propType.id, isPropMultiple)) {
                            property = e.getSingle(propType.id);
                            options = ClassMetadataSerialiser::Options::IgnoreMandatoryFlags;
                        } else {
                            property = propertyFactory->create();
                            props.emplace_back(property);
                        }

                        ClassMetadata metadata = property->getMetadata();
                        serialiser.writeToMetadata(metadata, options);
                        serialiser.setStorage(nullptr);
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
                            PAX_LOG(Log::Level::Error, "Parsing \"Properties\": Error during adding properties! Dependencies could not be met!");
                            break;
                        }
                    }
                });
    }
}

#endif //POLYPROPYLENE_JSONENTITYPREFABIMPL_H

#endif