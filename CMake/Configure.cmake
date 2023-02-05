include(StaticRuntime)
set_static_runtime()
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

option(Json_BUILD_TEST     "Build the unit test program." ON)
option(Json_AUTO_RUN_TEST  "Automatically run the test program." OFF)

set(Utils_ExternalTarget TRUE)
set(Utils_TargetFolders  TRUE)
set(Utils_TargetName     Utils)
set(Utils_TargetGroup    Extern)
set(Utils_INCLUDE        ${Json_SOURCE_DIR}/Internal/Utils)
set(Utils_LIBRARY        ${Utils_TargetName})

