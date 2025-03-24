cmake_minimum_required(VERSION 3.22)

project(gkr-insights)

#
# gkr-basics
#
set(GKR_BASICS_TESTS OFF)
add_subdirectory(gkr-basics)

#
# self
#
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

add_subdirectory(libs)
add_subdirectory(apps)
