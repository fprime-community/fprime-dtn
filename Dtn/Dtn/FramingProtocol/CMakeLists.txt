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
    #"${CMAKE_CURRENT_LIST_DIR}/Framer.cpp"
    #"${CMAKE_CURRENT_LIST_DIR}/FramerHelper.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/DtnProtocol.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/bpchat.c"
)

register_fprime_module()

set(UT_SOURCE_FILES
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
