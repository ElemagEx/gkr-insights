add_library(gkr_comm)
set_target_properties(gkr_comm PROPERTIES DEFINE_SYMBOL BUILDING_GKR_COMM)

target_include_directories(gkr_comm PUBLIC ".")

target_link_libraries(gkr_comm PRIVATE gkr_inline gkr_core gkr_log)
target_link_libraries(gkr_comm PRIVATE websockets_shared)

target_sources(
    gkr_comm
    INTERFACE
    "gkr/comm/api.hpp"
)
target_sources(
    gkr_comm
    PUBLIC
    "gkr/comm/registry.hpp"
    "gkr/comm/provider.hpp"
    "gkr/comm/lws_context.hpp"
    "gkr/comm/lws_protocol.hpp"
)
target_sources(
    gkr_comm
    PRIVATE
    "gkr/comm/registry.cpp"
    "gkr/comm/lws_context.cpp"
    "gkr/comm/lws_protocol.cpp"
)
