cmake_minimum_required(VERSION 3.22)

project(gkr-insights)

#
# libwebsockets
#
find_package(libwebsockets CONFIG REQUIRED)
list(APPEND CMAKE_MODULE_PATH ${LWS_CMAKE_DIR})
set(requirements 1)
require_lws_config(LWS_ROLE_WS 1 requirements)
require_lws_config(LWS_WITH_SERVER 1 requirements)

#
# gkr-basics
#
set(GKR_BASICS_TESTS OFF)
add_subdirectory(gkr-basics)

#
# self
#
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

#add_subdirectory(libs)
add_subdirectory(apps)
