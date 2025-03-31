add_library(gkr_comm)
set_target_properties(gkr_comm PROPERTIES DEFINE_SYMBOL BUILDING_GKR_COMM)

target_include_directories(gkr_comm PUBLIC ".")

target_link_libraries(gkr_comm PRIVATE gkr_inline gkr_core gkr_log)
target_link_libraries(gkr_comm PRIVATE websockets_shared)

target_sources(
    gkr_comm
    INTERFACE
    "gkr/capi/comm/api.h"
    "gkr/capi/comm/names.h"
    "gkr/capi/comm/registry.h"
    "gkr/capi/comm/parameters.h"
)
target_sources(
    gkr_comm
    INTERFACE
    "gkr/comm/api.hpp"
    "gkr/comm/registry.hpp"
    "gkr/comm/end_point.hpp"
    "gkr/comm/provider.hpp"
    "gkr/comm/service.hpp"
    "gkr/comm/bridge.hpp"
    "gkr/comm/params.hpp"
    "gkr/comm/names.hpp"
)
target_sources(
    gkr_comm
    PRIVATE
    "gkr/comm/registry.cpp"
    "gkr/comm/bridge.cpp"
)
target_sources(
    gkr_comm
    PUBLIC
    "gkr/comm/providers/lws_context.hpp"
    "gkr/comm/providers/lws_protocol.hpp"
)
target_sources(
    gkr_comm
    PRIVATE
    "gkr/comm/providers/lws_context.cpp"
    "gkr/comm/providers/lws_protocol.cpp"
)
