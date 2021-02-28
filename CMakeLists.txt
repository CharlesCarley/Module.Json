cmake_minimum_required(VERSION 3.0)
project(Json)

# Set the module path to include scripts
list(APPEND CMAKE_MODULE_PATH  ${Json_SOURCE_DIR}/CMake)
include(Configure)

if (Json_ExternalTarget)

    set(TargetFolders ${Json_TargetFolders})
    set(TargetName    ${Json_TargetName})
    set(TargetGroup   ${Json_TargetGroup})
else()
    subdirs(CMake)

    set(TargetFolders FALSE)
    set(TargetName Json)

    set(Json_INCLUDE  ${Json_SOURCE_DIR}/Source)
    set(Json_LIBRARY  ${TargetName})

endif()

subdirs(Source)

if (Json_BUILD_TEST)
    set(BUILD_GMOCK   OFF CACHE BOOL "" FORCE)
    set(INSTALL_GTEST OFF CACHE BOOL "" FORCE)

    subdirs(Test/googletest)
    subdirs(Test)
endif()
