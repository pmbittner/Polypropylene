set(SOURCE_FILES
        event/Event.cpp
        event/EventService.cpp

        io/Path.cpp

        property/Entity.cpp
        property/Prefab.cpp

        serialisation/ClassMetadataSerialiser.cpp
        serialisation/FieldStorage.cpp

        stdutils/StringUtils.cpp

        log/Log.cpp

        memory/Allocator.cpp
        memory/AllocationService.cpp
        memory/PropertyPool.cpp
        memory/allocators/MallocAllocator.cpp
        memory/allocators/PoolAllocator.cpp

        reflection/ClassMetadata.cpp
        reflection/Field.cpp
        reflection/VariableRegister.cpp
        reflection/Type.cpp)

if (POLYPROPYLENE_WITH_JSON)
    set(SOURCE_FILES ${SOURCE_FILES}
            serialisation/json/JsonUtil.cpp
            serialisation/json/JsonLoader.cpp
            serialisation/json/JsonTypeConverter.cpp
            serialisation/json/JsonFieldStorage.cpp
            serialisation/json/JsonFieldWriterRegister.cpp)
endif(POLYPROPYLENE_WITH_JSON)

add_library(polypropylene ${SOURCE_FILES})
add_library(Polypropylene::polypropylene ALIAS polypropylene)

target_include_directories(polypropylene PUBLIC
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)
target_compile_features(polypropylene PUBLIC cxx_std_17)

# These macros change the content of the public headers, so every consumer must see them.
target_compile_definitions(polypropylene PUBLIC
        $<$<BOOL:${POLYPROPYLENE_WITH_JSON}>:PAX_WITH_JSON>
        $<$<CONFIG:Debug>:PAX_BUILD_TYPE_DEBUG>
        $<$<CONFIG:Release>:PAX_BUILD_TYPE_RELEASE>)
