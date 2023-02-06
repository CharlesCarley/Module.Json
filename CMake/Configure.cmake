include(StaticRuntime)
include(ExternalTarget)

set_static_runtime()
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

option(Json_BUILD_TEST     "Build the unit test program." ON)
option(Json_AUTO_RUN_TEST  "Automatically run the test program." OFF)
option(Json_JUST_MY_CODE   "Enable the /JMC flag" ON)

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




set(ExtraFlags )
if (MSVC)
    # globally disable scoped enum warnings
    set(ExtraFlags "${ExtraFlags} /wd26812")
    
    
    set(ExtraFlags "${ExtraFlags} /W3")


    if (Json_JUST_MY_CODE)
        # Enable just my code...
        set(ExtraFlags "${ExtraFlags} /JMC")
    endif ()
    set(ExtraFlags "${ExtraFlags} /fp:precise")
    set(ExtraFlags "${ExtraFlags} /fp:except")
    set(ExtraFlags "${ExtraFlags} /openmp")

    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${ExtraFlags}")

else ()
    set(ExtraFlags "${ExtraFlags} -Os")
    set(ExtraFlags "${ExtraFlags} -O3")
    set(ExtraFlags "${ExtraFlags} -fPIC")
    
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${ExtraFlags}")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} stdc++fs")
endif ()

message(STATUS "Extra global flags: ${ExtraFlags}")
message(STATUS "Global flags: ${CMAKE_CXX_FLAGS}")

# Flag that the configure did not succeed
set(Configure_SUCCEEDED TRUE)