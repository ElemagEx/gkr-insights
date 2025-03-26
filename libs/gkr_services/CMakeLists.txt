add_library(gkr_server_comm)
set_target_properties(gkr_server_comm PROPERTIES DEFINE_SYMBOL BUILDING_GKR_SERVER_COMM)

target_include_directories(gkr_server_comm PUBLIC ".")

target_link_libraries(gkr_server_comm PRIVATE gkr_inline gkr_core gkr_log)
target_link_libraries(gkr_server_comm PUBLIC  gkr_comm)

target_link_libraries(gkr_server_comm PRIVATE websockets_shared)

target_sources(
    gkr_server_comm
    PUBLIC
    "gkr/comm/server_registrator.hpp"
)
target_sources(
    gkr_server_comm
    PRIVATE
    "gkr/comm/server_registrator.cpp"
)
target_sources(
    gkr_server_comm
    PRIVATE
    "gkr/comm/providers/lws_server.hpp"
    "gkr/comm/providers/lws_log_sink.hpp"
)
target_sources(
    gkr_server_comm
    PRIVATE
    "gkr/comm/providers/lws_server.cpp"
    "gkr/comm/providers/lws_log_sink.cpp"
)
