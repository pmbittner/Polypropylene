set(POLYPROPYLENE_INCLUDE_DIR ../../include/polypropylene)

set(HEADERS_FOR_CLION
        event/Delegate.h
        event/Event.h
        event/EventHandler.h
        event/EventService.h

        io/FileTypeChecker.h
        io/Path.h
        io/Settings.h

        log/Errors.h
        log/Log.h

        macros/CompilerDetection.h
        macros/Definitions.h
        macros/OSDetection.h

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
        property/construction/PropertyFactory.h
        property/construction/ContentProvider.h
        property/construction/EntityPrefab.h

        system/System.h
        system/SystemManager.h
        system/PropertyOwningSystem.h

        reflection/TemplateTypeToString.h
        reflection/TypeHandle.h
        reflection/TypeMap.h
        reflection/ClassMetadata.h
        reflection/Field.h

        resources/Resources.h
        resources/ResourceHandle.h
        resources/ResourceLoader.h

        stdutils/CollectionUtils.h

        Dependency.h
        Signature.h
        StringUtils.h
        StringVariables.h
        TryParser.h
        json/JsonFwd.h)

set(SOURCE_FILES
        Signature.cpp
        StringUtils.cpp
        StringVariables.cpp

        event/Event.cpp
        event/EventService.cpp

        io/Path.cpp
        io/Settings.cpp

        property/Entity.cpp
        property/construction/ContentProvider.cpp
        property/construction/EntityPrefab.cpp

        resources/Resources.cpp
        resources/ResourceLoader.cpp
        resources/ResourceHandle.cpp

        log/Log.cpp

        reflection/ClassMetadata.cpp
        reflection/Field.cpp)

if (POLYPROPYLENE_WITH_JSON)
    set(HEADERS_FOR_CLION ${HEADERS_FOR_CLION}
            json/JsonLoader.h
            json/JsonParser.h
            json/JsonUtil.h

            property/construction/json/JsonEntityPrefab.h
            property/construction/json/JsonEntityPrefabLoader.h
            property/construction/json/JsonPropertyContent.h
            )

    set(SOURCE_FILES ${SOURCE_FILES}
            json/JsonUtil.cpp
            json/JsonLoader.cpp
            property/construction/json/JsonPropertyContent.cpp system/System.cpp json/JsonParser.cpp)
endif(POLYPROPYLENE_WITH_JSON)

PAXPREPEND(HEADERS_FOR_CLION ${POLYPROPYLENE_INCLUDE_DIR} ${HEADERS_FOR_CLION})

add_library(polypropylene ${HEADERS_FOR_CLION} ${SOURCE_FILES})