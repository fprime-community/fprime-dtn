cmake_minimum_required(VERSION 3.13)
cmake_policy(SET CMP0048 NEW)
project(ltp-test VERSION 0.0.1 LANGUAGES C CXX)

#
# ION
#

foreach(lib
    bp
    ici
    ltp
)
    find_library(lib_name "lib${lib}.a" PATHS ${CMAKE_CURRENT_LIST_DIR}/../Dtn/Dtn/lib/ion REQUIRED)
    add_library(${lib} STATIC IMPORTED)
    set_target_properties(${lib} PROPERTIES IMPORTED_LOCATION ${lib_name})
    target_include_directories(${lib} INTERFACE ${CMAKE_CURRENT_LIST_DIR}/../Dtn/Dtn/include/ion)
    unset(lib_name CACHE)
endforeach()

#
# EXECUTABLE
#

find_package(Threads REQUIRED)

add_executable(ltp-test
    Main.cpp
)

target_link_libraries(ltp-test PRIVATE
    Threads::Threads
    bp
    ici
    ltp
)
