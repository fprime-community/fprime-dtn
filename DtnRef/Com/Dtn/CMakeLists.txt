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

#
# SUBDIRECTORIES
#

add_fprime_subdirectory("${CMAKE_CURRENT_LIST_DIR}/DtnDeframer/")
add_fprime_subdirectory("${CMAKE_CURRENT_LIST_DIR}/DtnFramer/")
