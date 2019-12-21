set(POLYPROPYLENE_INCLUDE_DIR ../../include/polypropylene)

set(HEADERS_FOR_CLION
        event/Delegate.h
        event/Event.h
        event/EventHandler.h
        event/EventService.h

        io/FileTypeChecker.h
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

        property/Property.h
        property/PropertyAnnotations.h
        property/Entity.h
        property/EntityManager.h
        property/EntityManagerView.h
        property/PropertyDependencies.h
        property/PropertyFactory.h
        property/EntityPrefab.h

        serialisation/ContentProvider.h
        serialisation/TryParser.h
        serialisation/json/JsonFwd.h

        system/System.h
        system/SystemManager.h
        system/PropertyOwningSystem.h

        reflection/TemplateTypeToString.h
        reflection/TypeHandle.h
        reflection/TypeMap.h
        reflection/ClassMetadata.h
        reflection/Field.h
        reflection/StringVariables.h

        resources/Resources.h
        resources/ResourceHandle.h
        resources/ResourceLoader.h
        resources/Signature.h

        stdutils/CollectionUtils.h
        stdutils/StringUtils.h

        Dependency.h)

set(SOURCE_FILES
        resources/Signature.cpp
        stdutils/StringUtils.cpp
        reflection/StringVariables.cpp

        event/Event.cpp
        event/EventService.cpp

        io/Path.cpp

        property/Entity.cpp
        serialisation/ContentProvider.cpp
        property/EntityPrefab.cpp

        resources/Resources.cpp
        resources/ResourceLoader.cpp
        resources/ResourceHandle.cpp

        system/System.cpp

        log/Log.cpp

        reflection/ClassMetadata.cpp
        reflection/Field.cpp)

if (POLYPROPYLENE_WITH_JSON)
    set(HEADERS_FOR_CLION ${HEADERS_FOR_CLION}
            serialisation/json/JsonLoader.h
            serialisation/json/JsonParser.h
            serialisation/json/JsonUtil.h

            serialisation/json/property/JsonEntityPrefab.h
            serialisation/json/property/JsonEntityPrefabLoader.h
            serialisation/json/property/JsonPropertyContent.h
            )

    set(SOURCE_FILES ${SOURCE_FILES}
            serialisation/json/JsonUtil.cpp
            serialisation/json/JsonLoader.cpp
            serialisation/json/JsonParser.cpp
            serialisation/json/property/JsonPropertyContent.cpp)
endif(POLYPROPYLENE_WITH_JSON)

PAXPREPEND(HEADERS_FOR_CLION ${POLYPROPYLENE_INCLUDE_DIR} ${HEADERS_FOR_CLION})

add_library(polypropylene ${HEADERS_FOR_CLION} ${SOURCE_FILES})