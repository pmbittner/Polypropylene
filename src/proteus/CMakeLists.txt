set(HEADERS_FOR_CLION
        ../../include/proteus/event/Delegate.h
        ../../include/proteus/event/Event.h
        ../../include/proteus/event/EventHandler.h
        ../../include/proteus/event/EventService.h

        ../../include/proteus/genericfactory/GenericFactory.h
        ../../include/proteus/genericfactory/GenericFactoryService.h

        ../../include/proteus/io/FileTypeChecker.h
        ../../include/proteus/io/Path.h
        ../../include/proteus/io/Settings.h

        ../../include/proteus/json/JsonLoader.h
        ../../include/proteus/json/JsonParser.h
        ../../include/proteus/json/JsonUtil.h

        ../../include/proteus/log/Errors.h
        ../../include/proteus/log/Log.h

        ../../include/proteus/macros/CompilerDetection.h
        ../../include/proteus/macros/Definitions.h
        ../../include/proteus/macros/OSDetection.h

        ../../include/proteus/memory/Allocator.h
        ../../include/proteus/memory/AllocationService.h
        ../../include/proteus/memory/allocators/MallocAllocator.h
        ../../include/proteus/memory/allocators/PoolAllocator.h

        ../../include/proteus/property/Property.h
        ../../include/proteus/property/PropertyAnnotations.h
        ../../include/proteus/property/PropertyContainer.h
        ../../include/proteus/property/PropertyContainerManager.h
        ../../include/proteus/property/PropertyContainerManagerView.h
        ../../include/proteus/property/PropertyDependencies.h
        ../../include/proteus/property/construction/PropertyFactory.h
        ../../include/proteus/property/construction/ContentProvider.h
        ../../include/proteus/property/construction/PropertyContainerPrefab.h
        ../../include/proteus/property/construction/json/JsonPropertyContainerPrefab.h
        ../../include/proteus/property/construction/json/JsonPropertyContainerPrefabLoader.h
        ../../include/proteus/property/construction/json/JsonPropertyContent.h

        ../../include/proteus/reflection/TemplateTypeToString.h
        ../../include/proteus/reflection/TypeHandle.h
        ../../include/proteus/reflection/TypeMap.h
        ../../include/proteus/reflection/TypeRegister.h

        ../../include/proteus/resources/Resources.h
        ../../include/proteus/resources/ResourceHandle.h
        ../../include/proteus/resources/ResourceLoader.h

        ../../include/proteus/stdutils/CollectionUtils.h

        ../../include/proteus/Dependency.h
        ../../include/proteus/Signature.h
        ../../include/proteus/StringUtils.h
        ../../include/proteus/StringVariables.h
        ../../include/proteus/TryParser.h
        ../../include/proteus/json/JsonFwd.h)

set(SOURCE_FILES
        StringUtils.cpp
        io/Path.cpp
        property/PropertyContainer.cpp
        io/Settings.cpp
        reflection/TypeRegister.cpp
        property/construction/ContentProvider.cpp
        json/JsonLoader.cpp
        property/construction/json/JsonPropertyContent.cpp
        event/EventService.cpp
        StringVariables.cpp
        resources/ResourceLoader.cpp
        resources/ResourceHandle.cpp
        Signature.cpp
        resources/Resources.cpp
        log/Log.cpp
        property/construction/PropertyContainerPrefab.cpp
        json/JsonUtil.cpp event/Event.cpp)

add_library(proteus ${HEADERS_FOR_CLION} ${SOURCE_FILES})