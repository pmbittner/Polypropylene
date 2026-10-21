cmake_minimum_required(VERSION 3.21)
project(Polypropylene
        VERSION 1.0.0
        DESCRIPTION "Entity-property system for C++"
        LANGUAGES CXX)

include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

### OPTIONS #############################################

# Examples and tests are only built when Polypropylene is the top-level project.
# When it is consumed as a dependency, they are off by default.
option(POLYPROPYLENE_WITH_JSON "Enable entity prefab loading from json files" ON)
option(POLYPROPYLENE_WITH_EXAMPLES "Build examples" ${PROJECT_IS_TOP_LEVEL})
option(POLYPROPYLENE_WITH_TESTS "Build unit tests; Requires POLYPROPYLENE_WITH_EXAMPLES=ON" ${PROJECT_IS_TOP_LEVEL})

# Building tests also requires the examples
if (POLYPROPYLENE_WITH_TESTS AND NOT POLYPROPYLENE_WITH_EXAMPLES)
    message(FATAL_ERROR "Building Tests (POLYPROPYLENE_WITH_TESTS) requires examples but POLYPROPYLENE_WITH_EXAMPLES is set to OFF.")
endif()

message(STATUS "Polypropylene: json=${POLYPROPYLENE_WITH_JSON} examples=${POLYPROPYLENE_WITH_EXAMPLES} tests=${POLYPROPYLENE_WITH_TESTS}")

### LIBRARY #############################################

add_subdirectory(src)

### EXAMPLES AND TESTS ##################################

if (POLYPROPYLENE_WITH_EXAMPLES)
    add_subdirectory(examples)
endif()

if (POLYPROPYLENE_WITH_TESTS)
    enable_testing()
    add_subdirectory(test)
endif()

### INSTALL #############################################

install(TARGETS polypropylene
        EXPORT PolypropyleneTargets
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
install(DIRECTORY include/polypropylene DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install(EXPORT PolypropyleneTargets
        NAMESPACE Polypropylene::
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/Polypropylene)

configure_package_config_file(
        cmake/PolypropyleneConfig.cmake.in
        ${CMAKE_CURRENT_BINARY_DIR}/PolypropyleneConfig.cmake
        INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/Polypropylene)
write_basic_package_version_file(
        ${CMAKE_CURRENT_BINARY_DIR}/PolypropyleneConfigVersion.cmake
        VERSION ${PROJECT_VERSION}
        COMPATIBILITY SameMajorVersion)
install(FILES
        ${CMAKE_CURRENT_BINARY_DIR}/PolypropyleneConfig.cmake
        ${CMAKE_CURRENT_BINARY_DIR}/PolypropyleneConfigVersion.cmake
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/Polypropylene)
