cmake_minimum_required(VERSION 3.22)

project(gkr-insights)

set(GKR_BASICS_TESTS OFF)
add_subdirectory(gkr-basics)

find_package(libwebsockets CONFIG REQUIRED)
list(APPEND CMAKE_MODULE_PATH ${LWS_CMAKE_DIR})
include(CheckCSourceCompiles)

set(requirements 1)
require_lws_config(LWS_ROLE_WS 1 requirements)
require_lws_config(LWS_WITH_SERVER 1 requirements)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

add_subdirectory(libs)

add_executable(gkr_main "main.cpp" "minimal-ws-broker.c")

if(requirements)
    #target_include_directories(gkr_main
    target_link_libraries(gkr_main gkr_core gkr_log gkr_mediator)
    target_link_libraries(gkr_main)#(websockets_shared)
    #add_dependencies(gkr_main websockets_shared)
    #target_link_libraries(gkr_main libweb

    #if (websockets_shared)
        #target_link_libraries(gkr_main websockets_shared ${LIBWEBSOCKETS_DEP_LIBS})
    #else()
        #target_link_libraries(gkr_main websockets) #${LIBWEBSOCKETS_DEP_LIBS}
    #endif()
else()
    message(FATAL_ERROR "Requirements are not met")
endif()
