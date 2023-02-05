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

include(${Json_SOURCE_DIR}/CMake/Configure.cmake)

if (Json_ExternalTarget)
    set(TargetFolders ${Json_TargetFolders})
    set(TargetName    ${Json_TargetName})
    set(TargetGroup   ${Json_TargetGroup})
else()
    add_subdirectory(CMake)
    add_subdirectory(Internal/Utils)

    set(TargetFolders TRUE)

    set(Json_INCLUDE  ${Json_SOURCE_DIR}/Source)
    set(Json_LIBRARY  ${TargetName})

endif()

add_subdirectory(Json)

if (Json_BUILD_TEST)
    if (NOT Json_ExternalTarget)
        set(TargetGroup Extern/ThirdParty)

        add_subdirectory(${GTEST_DIR})
        set_target_properties(gtest_main PROPERTIES FOLDER "${TargetGroup}")
        set_target_properties(gtest      PROPERTIES FOLDER "${TargetGroup}")
    endif()

    set(TargetGroup Units)
    add_subdirectory(Test)
endif()
