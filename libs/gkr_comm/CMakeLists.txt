#
# libwebsockets
#
find_package(libwebsockets CONFIG REQUIRED)
list(APPEND CMAKE_MODULE_PATH ${LWS_CMAKE_DIR})
set(requirements 1)
require_lws_config(LWS_ROLE_WS 1 requirements)
require_lws_config(LWS_WITH_SERVER 1 requirements)

add_library(gkr_comm)

target_include_directories(gkr_comm PUBLIC ".")

target_sources(
    gkr_comm
    INTERFACE
    "gkr/comm/api.hpp"
    "gkr/comm/lws_context.hpp"
    "gkr/comm/lws_protocol.hpp"
)
target_sources(
    gkr_comm
    PRIVATE
    "gkr/comm/lws_context.cpp"
    "gkr/comm/lws_protocol.cpp"
)

target_link_libraries(gkr_comm PUBLIC gkr_inline gkr_core gkr_log)

target_link_libraries(gkr_comm PRIVATE websockets_shared)

set_target_properties(gkr_comm PROPERTIES DEFINE_SYMBOL BUILDING_GKR_COMM_X)
