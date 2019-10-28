cmake_minimum_required(VERSION 3.10)

include(ExternalProject)
include(cmake/aquire_doctest.cmake)

option(BUILD_WITH_ASAN "Whether to build tests with ASAN" ON)

project(colibra)

set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)

add_library(colibra INTERFACE)
target_include_directories(colibra
    INTERFACE
        include
)
target_compile_features(colibra INTERFACE cxx_std_17)

enable_testing()
add_executable(colibra_test
    test/test_vector.cpp
    test/test_matrix.cpp
)
target_compile_features(colibra_test PRIVATE cxx_std_17)
target_include_directories(colibra_test
    PUBLIC
        ${DOCTEST_INCLUDE_DIR}
)
target_link_libraries(colibra_test
    PUBLIC
        colibra
    PRIVATE
        $<$<BOOL:${BUILD_WITH_ASAN}>:-fno-omit-frame-pointer>
        $<$<BOOL:${BUILD_WITH_ASAN}>:-fsanitize=address>
)

add_dependencies(colibra_test doctest)
add_test(test_colibra colibra_test)
