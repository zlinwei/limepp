project(cppzmq-test CXX)
include(cmake/googletest.cmake)
fetch_googletest(
        ${PROJECT_SOURCE_DIR}/cmake
        ${PROJECT_BINARY_DIR}/googletest
)


add_executable(unit_tests unit_tests.cpp)

target_link_libraries(unit_tests
        PRIVATE gtest
        PRIVATE gmock_main
        PRIVATE LimeSDR
        )

add_test(TestAllCase tests)
