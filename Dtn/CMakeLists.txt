cmake_minimum_required(VERSION 3.13)
cmake_policy(SET CMP0048 NEW)
project(Dtn VERSION 0.0.1 LANGUAGES C CXX)

#
# F PRIME
#

include("${FPRIME_FRAMEWORK_PATH}/cmake/FPrime.cmake")
# NOTE: register custom targets between these two lines
include("${FPRIME_FRAMEWORK_PATH}/cmake/FPrime-Code.cmake")

#
# SUBDIRECTORIES
#

add_fprime_subdirectory("${CMAKE_CURRENT_LIST_DIR}/Top/")

include("${CMAKE_CURRENT_LIST_DIR}/Dtn.cmake")

set(SOURCE_FILES "${CMAKE_CURRENT_LIST_DIR}/Top/Main.cpp")
set(MOD_DEPS ${PROJECT_NAME}/Top)

register_fprime_deployment()
