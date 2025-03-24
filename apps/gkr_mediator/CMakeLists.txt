add_executable(gkr_mediator "main.cpp")

target_link_libraries(gkr_mediator PRIVATE gkr_inline gkr_core gkr_log)
#target_link_libraries(gkr_mediator PRIVATE gkr_services)

target_sources(
    gkr_mediator
    INTERFACE
    "lws_server.cpp"
    "lws_log_sink.cpp"
)
target_sources(
    gkr_mediator
    PRIVATE
    "lws_server.hpp"
    "lws_log_sink.hpp"
)
