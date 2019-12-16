set(POLYPROPYLENE_INCLUDE_DIR ../../include/polypropylene)

set(HEADERS_FOR_CLION
        event/Delegate.h
        event/Event.h
        event/EventHandler.h
        event/EventService.h

        genericfactory/GenericFactory.h
        genericfactory/GenericFactoryService.h

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

        reflection/TemplateTypeToString.h
        reflection/TypeHandle.h
        reflection/TypeMap.h
        reflection/TypeRegister.h

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
        StringUtils.cpp
        io/Path.cpp
        property/Entity.cpp
        io/Settings.cpp
        reflection/TypeRegister.cpp
        property/construction/ContentProvider.cpp
        event/EventService.cpp
        StringVariables.cpp
        resources/ResourceLoader.cpp
        resources/ResourceHandle.cpp
        Signature.cpp
        resources/Resources.cpp
        log/Log.cpp
        property/construction/EntityPrefab.cpp
        event/Event.cpp)

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
            property/construction/json/JsonPropertyContent.cpp)
endif(POLYPROPYLENE_WITH_JSON)

PAXPREPEND(HEADERS_FOR_CLION ${POLYPROPYLENE_INCLUDE_DIR} ${HEADERS_FOR_CLION})

add_library(polypropylene ${HEADERS_FOR_CLION} ${SOURCE_FILES})