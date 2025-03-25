add_library(gkr_pipes)
set_target_properties(gkr_pipes PROPERTIES DEFINE_SYMBOL BUILDING_GKR_PIPES)

target_include_directories(gkr_pipes PUBLIC ".")

target_link_libraries(gkr_pipes PRIVATE gkr_inline gkr_core gkr_log)
target_link_libraries(gkr_pipes PUBLIC  gkr_comm)

target_link_libraries(gkr_pipes PRIVATE websockets_shared)#???

target_sources(
    gkr_pipes
    INTERFACE
    "gkr/pipes/api.hpp"
    "gkr/pipes/client_factory.hpp"
    "gkr/pipes/lws_client.hpp"
    "gkr/pipes/lws_log_pipe.hpp"
    "gkr/pipes/lws_log_consumer.hpp"
)
target_sources(
    gkr_pipes
    PRIVATE
    "gkr/pipes/client_factory.cpp"
    "gkr/pipes/lws_client.cpp"
    "gkr/pipes/lws_log_pipe.cpp"
    "gkr/pipes/lws_log_consumer.cpp"
)
