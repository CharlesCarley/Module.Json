if (NOT MSVC)
    return()
endif()

include(GroupSet)
set(TargetName BuildConfig)
set(GroupRoot ../)

set_group(TargetName_SRC
    :${GroupRoot}/.clang-format
    :${GroupRoot}/.clang-format
    :${GroupRoot}/CMakeLists.txt
    Config:${GroupRoot}/CMake/CMakeLists.txt
    Config:${GroupRoot}/CMake/*.cmake
    Config/Modules:${GroupRoot}/CMake/Modules/*.cmake
)

set_source_files_properties(${TargetName_SRC} PROPERTIES VS_TOOL_OVERRIDE "None")

add_custom_target(
    ${TargetName} 
    ALL SOURCES ${TargetName_SRC}
    )


set_target_properties(${TargetName} PROPERTIES FOLDER "${TargetGroup}")
