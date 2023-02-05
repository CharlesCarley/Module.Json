if (Json_ExternalTarget)
    set(TargetFolders ${Json_TargetFolders})
    set(TargetName    ${Json_TargetName})
    set(TargetGroup   ${Json_TargetGroup})
else()
    set(TargetName Json)
endif()
include(GroupSet)


set_group(Target_SRC
    Source:*.cpp
    Header:*.h
)
include_directories(${Utils_INCLUDE} .)

add_library(${TargetName}  ${Target_SRC})
target_link_libraries(${TargetName} ${Utils_LIBRARY})
set_target_properties(${TargetName} PROPERTIES FOLDER "${TargetGroup}")
