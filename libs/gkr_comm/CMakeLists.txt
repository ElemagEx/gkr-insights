add_library(gkr_comm)
set_target_properties(gkr_comm PROPERTIES DEFINE_SYMBOL BUILDING_GKR_COMM)

target_include_directories(gkr_comm PUBLIC ".")

target_link_libraries(gkr_comm PRIVATE gkr_inline gkr_core gkr_log)
target_link_libraries(gkr_comm PRIVATE websockets_shared)

target_sources(
    gkr_comm
    INTERFACE
    "gkr/capi/comm/api.h"
    "gkr/capi/comm/registry.h"
    "gkr/capi/comm/constants.h"
)
target_sources(
    gkr_comm
    INTERFACE
    "gkr/comm/api.hpp"
    "gkr/comm/end_point.hpp"
    "gkr/comm/constants.hpp"
    "gkr/comm/provider.hpp"
    "gkr/comm/service.hpp"
    "gkr/comm/log_packet.hpp"
    "gkr/comm/log_collector.hpp"
)
target_sources(
    gkr_comm
    INTERFACE
    "gkr/comm/log.hxx"
    "gkr/comm/registry.hpp"
    "gkr/comm/bridge.hpp"
    "gkr/comm/log_receiver.hpp"
    "gkr/comm/app_console_log_collector.hpp"
    "gkr/comm/log_upstream_log_consumer.hpp"
)
target_sources(
    gkr_comm
    PRIVATE
    "gkr/comm/log.cpp"
    "gkr/comm/registry.cpp"
    "gkr/comm/bridge.cpp"
    "gkr/comm/log_receiver.cpp"
    "gkr/comm/app_console_log_collector.cpp"
    "gkr/comm/log_upstream_log_consumer.cpp"
)
target_sources(
    gkr_comm
    PUBLIC
    "gkr/comm/providers/lws_context.hpp"
    "gkr/comm/providers/lws_protocol.hpp"
    "gkr/comm/providers/lws_log_pipe.hpp"
    "gkr/comm/providers/lws_log_sink.hpp"
)
target_sources(
    gkr_comm
    PRIVATE
    "gkr/comm/providers/lws_context.cpp"
    "gkr/comm/providers/lws_protocol.cpp"
    "gkr/comm/providers/lws_log_pipe.cpp"
    "gkr/comm/providers/lws_log_sink.cpp"
)
