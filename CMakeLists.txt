cmake_minimum_required(VERSION 3.22)

project(gkr-insights)

set(GKR_BASICS_TESTS OFF)
add_subdirectory(gkr-basics)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

add_executable(gkr_main main.cpp)
target_link_libraries(gkr_main gkr_core)
