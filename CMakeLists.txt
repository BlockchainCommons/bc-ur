cmake_minimum_required(VERSION 3.15)
project(bc-ur
    VERSION 0.3.0
    DESCRIPTION "UR reference library in C++"
    LANGUAGES C CXX
)

option(ENABLE_TESTS "Build bc-ur tests" FALSE)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

add_library(bc-ur)

set(public_headers
    src/bc-ur.hpp
    src/bytewords.hpp
    src/cbor-lite.hpp
    src/fountain-decoder.hpp
    src/fountain-encoder.hpp
    src/fountain-utils.hpp
    src/random-sampler.hpp
    src/ur.hpp
    src/ur-decoder.hpp
    src/ur-encoder.hpp
    src/utils.hpp
    src/xoshiro256.hpp
)
set(sources
    src/bytewords.cpp
    src/crc32.c
    src/crc32.h
    src/fountain-decoder.cpp
    src/fountain-encoder.cpp
    src/fountain-utils.cpp
    src/Makefile.in
    src/memzero.c
    src/memzero.h
    src/random-sampler.cpp
    src/sha2.c
    src/sha2.h
    src/ur.cpp
    src/ur-decoder.cpp
    src/ur-encoder.cpp
    src/utils.cpp
    src/xoshiro256.cpp
)

target_sources(bc-ur PRIVATE ${public_headers} ${sources})
target_include_directories(bc-ur
    PUBLIC
        "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>"
    PRIVATE
        "${CMAKE_CURRENT_SOURCE_DIR}/src"
)
set_target_properties(bc-ur PROPERTIES
    PUBLIC_HEADER "${public_headers}"
    VERSION ${PROJECT_VERSION}
)

if(ENABLE_TESTS)
    enable_testing()
    add_executable(bc-ur-test test/test.cpp test/test-utils.cpp)
    target_include_directories(bc-ur-test PRIVATE test)
    target_link_libraries(bc-ur-test bc-ur)
    add_test(bc-ur-test ./bc-ur-test)
endif()

include(CMakePackageConfigHelpers)
include(GNUInstallDirs)
configure_package_config_file(cmake/bc-ur-config.cmake.in bc-ur-config.cmake
    INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/bc-ur
)
write_basic_package_version_file(
    ${CMAKE_CURRENT_BINARY_DIR}/bc-ur-config-version.cmake
    COMPATIBILITY SameMajorVersion
)
install(TARGETS bc-ur
    EXPORT "bc-ur-export"
    ARCHIVE
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/bc-ur
)
install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/bc-ur-config.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/bc-ur-config-version.cmake
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/bc-ur
)
install(EXPORT "bc-ur-export"
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/bc-ur
    NAMESPACE ${PROJECT_NAME}::
    FILE bc-ur-targets.cmake
)

