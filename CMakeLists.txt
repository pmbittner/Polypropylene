cmake_minimum_required(VERSION 3.6)
project(Polypropylene)

### DEFINITIONS #########################################

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)
include(cmake/Prepend.cmake)
include(cmake/OptionInfo.cmake)

### OPTIONS #############################################

option(POLYPROPYLENE_WITH_EXAMPLES "Build examples" ON)
option(POLYPROPYLENE_WITH_JSON "Enable entity prefab loading from json files" ON)
option(POLYPROPYLENE_WITH_TESTS "Build unit tests; Requires POLYPROPYLENE_WITH_EXAMPLES=ON" ON)

message("Building Polypropylene")
message("  FOR C++${CMAKE_CXX_STANDARD}")
printOptionInfo(POLYPROPYLENE_WITH_EXAMPLES Examples PAX_WITH_EXAMPLES)
printOptionInfo(POLYPROPYLENE_WITH_JSON Json PAX_WITH_JSON)
printOptionInfo(POLYPROPYLENE_WITH_TESTS Tests PAX_WITH_TESTS)

### OPTION CONSTRAINTS #################################

# Building tests also requires the examples
if (POLYPROPYLENE_WITH_TESTS AND NOT POLYPROPYLENE_WITH_EXAMPLES)
    message(FATAL_ERROR "Building Tests (POLYPROPYLENE_WITH_TESTS) requires examples but POLYPROPYLENE_WITH_EXAMPLES is set to OFF.")
endif()

### FLAGS ###############################################

if (${CMAKE_BUILD_TYPE} MATCHES Release)
    add_definitions(-DPAX_BUILD_TYPE_RELEASE)
    message("  AS Release")
elseif(${CMAKE_BUILD_TYPE} MATCHES Debug)
    add_definitions(-DPAX_BUILD_TYPE_DEBUG)
    message("  AS Debug")
endif(${CMAKE_BUILD_TYPE} MATCHES Release)

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