add_library(gkr_services)
set_target_properties(gkr_services PROPERTIES DEFINE_SYMBOL BUILDING_GKR_SERVICES)

target_include_directories(gkr_services PUBLIC ".")

target_link_libraries(gkr_services PRIVATE gkr_inline gkr_core gkr_log)
target_link_libraries(gkr_services PUBLIC  gkr_comm)

target_link_libraries(gkr_services PRIVATE websockets_shared)

target_sources(
    gkr_services
    PRIVATE
    "gkr/services/registrator.cpp"
)
target_sources(
    gkr_services
    PRIVATE
    "gkr/services/lws_server.hpp"
    "gkr/services/lws_log_sink.hpp"
)
target_sources(
    gkr_services
    PRIVATE
    "gkr/services/lws_server.cpp"
    "gkr/services/lws_log_sink.cpp"
)
