cmake_minimum_required(VERSION 3.22)

project(IIRFilter)

set(CMAKE_CXX_STANDARD 20)

set(LIB_DIR ${CMAKE_CURRENT_SOURCE_DIR}/libs)

include(cmake/cpm.cmake)

# Manually adding them for now
#add_subdirectory(${LIB_DIR}/juce)
#add_subdirectory(${LIB_DIR}/googletest)

CPMAddPackage(
    NAME JUCE
    GIT_TAG 8.0.2
    VERSION 8.0.2
    GITHUB_REPOSITORY juce-framework/JUCE
    SOURCE_DIR ${LIB_DIR}/juce
)

CPMAddPackage(
    NAME GOOGLETEST
    GITHUB_REPOSITORY google/googletest
    GIT_TAG v1.15.2
    VERSION 1.15.2
    SOURCE_DIR ${LIB_DIR}/googletest
    OPTIONS
        "INSTALL_GTEST OFF"
        "gtest_force_shared_crt ON"
)

include_directories(${LIB_DIR}/juce/modules)

enable_testing()

add_subdirectory(plugin)
