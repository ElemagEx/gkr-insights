add_library(gkr_pipes)
set_target_properties(gkr_pipes PROPERTIES DEFINE_SYMBOL BUILDING_GKR_PIPES)

target_include_directories(gkr_pipes PUBLIC ".")

target_link_libraries(gkr_pipes PRIVATE gkr_inline gkr_core gkr_log)
target_link_libraries(gkr_pipes PUBLIC  gkr_comm)

target_link_libraries(gkr_pipes PRIVATE websockets_shared)#???

target_sources(
    gkr_pipes
    PUBLIC
    "gkr/comm/client_registrator.hpp"
    "gkr/comm/web_socket_log_consumer.hpp"
)
target_sources(
    gkr_pipes
    PRIVATE
    "gkr/comm/client_registrator.cpp"
    "gkr/comm/web_socket_log_consumer.cpp"
)
target_sources(
    gkr_pipes
    PRIVATE
    "gkr/comm/providers/lws_client.hpp"
    "gkr/comm/providers/lws_log_pipe.hpp"
)
target_sources(
    gkr_pipes
    PRIVATE
    "gkr/comm/providers/lws_client.cpp"
    "gkr/comm/providers/lws_log_pipe.cpp"
)
