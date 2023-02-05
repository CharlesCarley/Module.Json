cmake_minimum_required(VERSION 3.15)
project(Json)
# Use C++-17 by default.
enable_language(CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


# Set the module path to include scripts
list(APPEND 
    CMAKE_MODULE_PATH  
    ${Json_SOURCE_DIR}/CMake          # (specific) Local to project
    ${Json_SOURCE_DIR}/CMake/Modules  # (generic)  Global to all projects
)
include(Configure)

if (Json_ExternalTarget)
    set(TargetFolders ${Json_TargetFolders})
    set(TargetName    ${Json_TargetName})
    set(TargetGroup   ${Json_TargetGroup})
else()
    add_subdirectory(CMake)
    add_subdirectory(Internal/Utils/Utils)

    set(TargetFolders TRUE)

    set(Json_INCLUDE  ${Json_SOURCE_DIR}/Source)
    set(Json_LIBRARY  ${TargetName})

endif()

add_subdirectory(Json)

if (Json_BUILD_TEST)
    set(GTEST_INCLUDE ${Json_SOURCE_DIR}/Test/googletest/googletest/include)
    set(GTEST_LIBRARY gtest_main)

    set(BUILD_GMOCK   OFF CACHE BOOL "" FORCE)
    set(INSTALL_GTEST OFF CACHE BOOL "" FORCE)

    add_subdirectory(Test/googletest)
    add_subdirectory(Test)
endif()
