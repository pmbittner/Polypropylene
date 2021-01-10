cmake_minimum_required(VERSION 3.6)
project(Polypropylene)

### DEFINITIONS #########################################
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)
option(POLYPROPYLENE_WITH_EXAMPLES "Build examples" ON)
option(POLYPROPYLENE_WITH_JSON "Enable entity prefab loading from json files" ON)
option(POLYPROPYLENE_WITH_TESTS "Build unit tests" ON)

### FLAGS ###############################################
set(CMAKE_CXX_STANDARD 17)
message("CMAKE_CXX_STANDARD is ${CMAKE_CXX_STANDARD}")
include(cmake/Prepend.cmake)

if (${CMAKE_BUILD_TYPE} MATCHES Release)
    add_definitions(-DPAX_BUILD_TYPE_RELEASE)
    message("BUILD TYPE is Release")
elseif(${CMAKE_BUILD_TYPE} MATCHES Debug)
    add_definitions(-DPAX_BUILD_TYPE_DEBUG)
    message("BUILD TYPE is Debug")
endif(${CMAKE_BUILD_TYPE} MATCHES Release)

if (POLYPROPYLENE_WITH_JSON)
    add_definitions(-DPAX_WITH_JSON)
    message("Building WITH: Json (POLYPROPYLENE_WITH_JSON = ON)")
else()
    message("Building WITHOUT: Json (POLYPROPYLENE_WITH_JSON = OFF)")
endif()

if (POLYPROPYLENE_WITH_TESTS)
    add_definitions(-DPAX_WITH_TESTS)
    message("Building WITH: Tests (POLYPROPYLENE_WITH_TESTS = ON)")
else()
    message("Building WITHOUT: Tests (POLYPROPYLENE_WITH_TESTS = OFF)")
endif()

### SOURCE FILES ########################################

include_directories(include)
add_subdirectory(src)

### EXAMPLES ############################################

if (POLYPROPYLENE_WITH_EXAMPLES)
    add_subdirectory(examples)
endif(POLYPROPYLENE_WITH_EXAMPLES)

### TEST ################################################

if (POLYPROPYLENE_WITH_TESTS)
    add_subdirectory(test)
endif(POLYPROPYLENE_WITH_TESTS)