cmake_minimum_required(VERSION 3.13)
cmake_policy(SET CMP0048 NEW)
project(Dtn VERSION 0.0.1 LANGUAGES C CXX)
add_compile_options(-DPROTECTED=public) # TODO remove deframer hack

#
# F PRIME
#

include("${FPRIME_FRAMEWORK_PATH}/cmake/FPrime.cmake")
# NOTE: register custom targets between these two lines
include("${FPRIME_FRAMEWORK_PATH}/cmake/FPrime-Code.cmake")

#
# SUBDIRECTORIES
#

include("${CMAKE_CURRENT_LIST_DIR}/Dtn.cmake")
