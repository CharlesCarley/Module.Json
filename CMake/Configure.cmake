include(StaticRuntime)
include(ExternalTarget)

set_static_runtime()
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

option(Json_BUILD_TEST     "Build the unit test program." ON)
option(Json_AUTO_RUN_TEST  "Automatically run the test program." OFF)
option(Json_JUST_MY_CODE   "Enable the /JMC flag" ON)
option(Json_OPEN_MP        "Enable low-level fill and copy using OpenMP" ON)

set(ExternalTarget_LOG OFF)

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

    if (Json_OPEN_MP)
        add_definitions(-DRT_OPEN_MP=1)
        set(ExtraFlags "${ExtraFlags} /openmp")
    endif()

    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${ExtraFlags}")

else ()
    set(ExtraFlags "${ExtraFlags} -Os")
    set(ExtraFlags "${ExtraFlags} -O3")
    set(ExtraFlags "${ExtraFlags} -fPIC")

    if (Json_OPEN_MP)
        add_definitions(-DRT_OPEN_MP=1)
        set(ExtraFlags "${ExtraFlags} -fopenmp")
    endif()
    
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${ExtraFlags}")
endif ()

message(STATUS "Extra global flags: ${ExtraFlags}")
message(STATUS "Global flags: ${CMAKE_CXX_FLAGS}")

# Flag that the configure did not succeed
set(Configure_SUCCEEDED TRUE)