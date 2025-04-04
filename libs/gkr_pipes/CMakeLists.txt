add_library(gkr_client_comm)
set_target_properties(gkr_client_comm PROPERTIES DEFINE_SYMBOL BUILDING_GKR_CLIENT_COMM)

target_include_directories(gkr_client_comm PUBLIC ".")

target_link_libraries(gkr_client_comm PRIVATE gkr_inline gkr_core gkr_log)
target_link_libraries(gkr_client_comm PUBLIC  gkr_comm)

target_link_libraries(gkr_client_comm PRIVATE websockets_shared)

target_sources(
    gkr_client_comm
    INTERFACE
    "gkr/capi/comm/web_socket_log_consumer.h"
)
target_sources(
    gkr_client_comm
    PUBLIC
    "gkr/comm/client_registrator.hpp"
    "gkr/comm/upstream_log_consumer.hpp"
)
target_sources(
    gkr_client_comm
    PRIVATE
    "gkr/comm/client_registrator.cpp"
    "gkr/comm/upstream_log_consumer.cpp"
)
target_sources(
    gkr_client_comm
    PRIVATE
    "gkr/comm/providers/lws_client.hpp"
    "gkr/comm/providers/lws_log_pipe.hpp"
)
target_sources(
    gkr_client_comm
    PRIVATE
    "gkr/comm/providers/lws_client.cpp"
    "gkr/comm/providers/lws_log_pipe.cpp"
)
