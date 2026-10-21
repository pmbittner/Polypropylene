find_package(GTest REQUIRED)

add_executable(polypropylenetests polypropyleneTests.cpp)
target_include_directories(polypropylenetests PRIVATE include ../examples/pizza)
target_link_libraries(polypropylenetests PRIVATE GTest::gtest_main pizzalib)

include(GoogleTest)
gtest_discover_tests(polypropylenetests)
