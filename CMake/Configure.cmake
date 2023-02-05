include(StaticRuntime)
include(ExternalTarget)

set_static_runtime()
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

option(Json_BUILD_TEST     "Build the unit test program." ON)
option(Json_AUTO_RUN_TEST  "Automatically run the test program." OFF)

set(ExternalTarget_LOG ON)

set(BUILD_GMOCK   OFF CACHE BOOL "" FORCE)
set(INSTALL_GTEST OFF CACHE BOOL "" FORCE)
set(GTEST_DIR     ${Json_SOURCE_DIR}/Test/googletest)
set(GTEST_INCLUDE ${Json_SOURCE_DIR}/Test/googletest/googletest/include)
set(GTEST_LIBRARY gtest_main)


DefineExternalTargetEx(
    Utils Extern
    ${Json_SOURCE_DIR}/Internal/Utils 
    ${Json_SOURCE_DIR}/Internal/Utils
    ${Json_BUILD_TEST}
    ${Json_AUTO_RUN_TEST}
)
