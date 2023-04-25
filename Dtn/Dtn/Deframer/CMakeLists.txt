#
# LINK LIBRARIES
#

# ION

set(MOD_DEPS
    bp
    ici
    ltp
)

#
# REGISTER MODULE
#

set(SOURCE_FILES
    "${CMAKE_CURRENT_LIST_DIR}/Deframer.fpp"
    "${CMAKE_CURRENT_LIST_DIR}/Deframer.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/DeframerHelper.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/bp_receive.c"
)

register_fprime_module()

set(UT_SOURCE_FILES
    "${CMAKE_CURRENT_LIST_DIR}/Deframer.fpp"
    "${CMAKE_CURRENT_LIST_DIR}/test/ut/Tester.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/test/ut/TesterHelpers.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/test/ut/TestMain.cpp"
)

#
# REGISTER TESTS
#

set(UT_MOD_DEPS
    STest
)

register_fprime_ut()
