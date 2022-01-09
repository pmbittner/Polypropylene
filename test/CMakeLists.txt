# This file was created and customised according to the official googletest documentation at
# https://github.com/google/googletest/tree/master/googletest.

include(FetchContent)
FetchContent_Declare(
        googletest
        # Specify the commit you depend on and update it regularly.
        URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# Now simply link against gtest or gtest_main as needed. Eg
if (POLYPROPYLENE_WITH_EXAMPLES)
    add_executable(polypropylenetests polypropyleneTests.cpp)
    target_include_directories(polypropylenetests PUBLIC include)
    target_include_directories(polypropylenetests PUBLIC ../examples/pizza)
    target_link_libraries(polypropylenetests PUBLIC gtest_main pizzalib)
    add_test(NAME polypropylene_tests COMMAND polypropylenetests)
endif(POLYPROPYLENE_WITH_EXAMPLES)

