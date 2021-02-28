# -----------------------------------------------------------------------------
#   Copyright (c) Charles Carley.
#
#   This software is provided 'as-is', without any express or implied
# warranty. In no event will the authors be held liable for any damages
# arising from the use of this software.
#
#   Permission is granted to anyone to use this software for any purpose,
# including commercial applications, and to alter it and redistribute it
# freely, subject to the following restrictions:
#
# 1. The origin of this software must not be misrepresented; you must not
#    claim that you wrote the original software. If you use this software
#    in a product, an acknowledgment in the product documentation would be
#    appreciated but is not required.
# 2. Altered source versions must be plainly marked as such, and must not be
#    misrepresented as being the original software.
# 3. This notice may not be removed or altered from any source distribution.
# ------------------------------------------------------------------------------
if (Json_ExternalTarget)
    set(TargetFolders ${Json_TargetFolders})
    set(TargetName    ${Json_TargetName})
    set(TargetGroup   ${Json_TargetGroup})
else()
    set(TargetFolders FALSE)
    set(TargetName Json)
    unset(TargetGroup )
endif()



set(TargetName_SOURCE 
    skJsonArray.cpp
    skJsonObject.cpp
    skJsonParser.cpp
    skJsonScanner.cpp
    skJsonToken.cpp
    skJsonType.cpp

    skJsonBoolean.h
    skJsonArray.h
    skJsonDouble.h
    skJsonInteger.h
    skJsonObject.h
    skJsonParser.h
    skJsonPointer.h
    skJsonScanner.h
    skJsonString.h
    skJsonToken.h
    skJsonVisitor.h
    skJsonType.h
)

include_directories(${Utils_INCLUDE} .)

add_library(
    ${TargetName} 
    ${TargetName_SOURCE}
)

target_link_libraries( ${TargetName} ${Utils_LIBRARY})

if (TargetFolders)
    set_target_properties(${TargetName} PROPERTIES FOLDER "${TargetGroup}")
endif()
