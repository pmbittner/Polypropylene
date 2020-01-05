set(POLYPROPYLENE_INCLUDE_DIR ../include/polypropylene)

set(HEADERS_FOR_CLION
        Polypropylene.h
        Dependency.h
        Prefab.h

        event/Delegate.h
        event/Event.h
        event/EventHandler.h
        event/EventService.h

        io/Path.h

        log/Errors.h
        log/Log.h

        definitions/CompilerDetection.h
        definitions/Definitions.h
        definitions/OSDetection.h

        memory/Allocator.h
        memory/AllocationService.h
        memory/allocators/MallocAllocator.h
        memory/allocators/PoolAllocator.h

        property/ForwardDeclarations.h
        property/Property.h
        property/PropertyAnnotations.h
        property/Entity.h
        property/EntityManager.h
        property/EntityManagerView.h
        property/PropertyDependencies.h
        property/PropertyFactory.h
        property/PrototypeEntityPrefab.h

        serialisation/FieldStorage.h
        serialisation/ClassMetadataSerialiser.h
        serialisation/TryParser.h
        serialisation/json/JsonFwd.h

        system/System.h
        system/SystemManager.h
        system/PropertyOwningSystem.h

        reflection/Reflectable.h
        reflection/TemplateTypeToString.h
        reflection/TypeHandle.h
        reflection/TypeMap.h
        reflection/ClassMetadata.h
        reflection/Field.h
        reflection/VariableRegister.h

        stdutils/CollectionUtils.h
        stdutils/StringUtils.h)

set(SOURCE_FILES
        event/Event.cpp
        event/EventService.cpp

        io/Path.cpp

        property/Entity.cpp
        property/Prefab.cpp

        serialisation/ClassMetadataSerialiser.cpp
        serialisation/FieldStorage.cpp

        system/System.cpp

        stdutils/StringUtils.cpp

        log/Log.cpp

        reflection/ClassMetadata.cpp
        reflection/Field.cpp
        reflection/VariableRegister.cpp
        reflection/TypeHandle.cpp)

if (POLYPROPYLENE_WITH_JSON)
    set(HEADERS_FOR_CLION ${HEADERS_FOR_CLION}
            serialisation/json/JsonLoader.h
            serialisation/json/JsonParser.h
            serialisation/json/JsonUtil.h
            serialisation/json/JsonFieldStorage.h

            serialisation/json/property/JsonEntityPrefab.h
            serialisation/json/property/JsonEntityPrefabLoader.h
            serialisation/json/property/JsonEntityPrefabLoaderImpl.h
            serialisation/json/property/JsonEntityPrefabImpl.h)

    set(SOURCE_FILES ${SOURCE_FILES}
            serialisation/json/JsonUtil.cpp
            serialisation/json/JsonLoader.cpp
            serialisation/json/JsonParser.cpp
            serialisation/json/JsonFieldStorage.cpp)
endif(POLYPROPYLENE_WITH_JSON)

PAXPREPEND(HEADERS_FOR_CLION ${POLYPROPYLENE_INCLUDE_DIR} ${HEADERS_FOR_CLION})

add_library(polypropylene ${HEADERS_FOR_CLION} ${SOURCE_FILES})