#
# LINK LIBRARIES
#

# ION

include_directories(include/ion)

foreach(lib
    bp
    ici
    ltp
)
    find_library(lib_name "lib${lib}.a" PATHS lib/ion REQUIRED)
    add_library(${lib} STATIC IMPORTED)
    set_target_properties(${lib} PROPERTIES IMPORTED_LOCATION ${lib_name})
    unset(lib_name CACHE)
endforeach()

set(MOD_DEPS
    bp
    ici
    ltp
)

#
# REGISTER MODULE
#

set(SOURCE_FILES
    "${CMAKE_CURRENT_LIST_DIR}/Dtn.fpp"
    "${CMAKE_CURRENT_LIST_DIR}/Dtn.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/bpchat.c"
)

register_fprime_module()

set(UT_SOURCE_FILES
    "${CMAKE_CURRENT_LIST_DIR}/Dtn.fpp"
    "${CMAKE_CURRENT_LIST_DIR}/test/ut/Tester.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/test/ut/TestMain.cpp"
)

#
# REGISTER TESTS
#

set(UT_MOD_DEPS
    STest
)

register_fprime_ut()
