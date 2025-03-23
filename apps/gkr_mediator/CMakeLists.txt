add_executable(gkr_mediator "main.cpp" "lws_context.cpp" "lws_protocol.cpp" "lws_server.cpp" "lws_log_sink.cpp")

target_link_libraries(gkr_mediator PRIVATE gkr_core gkr_data gkr_comm gkr_log)
target_link_libraries(gkr_mediator PRIVATE websockets_shared)
#add_dependencies(gkr_mediator websockets_shared)
#target_include_directories(gkr_mediator PUBLIC ".")
#set_target_properties(gkr_mediator PROPERTIES DEFINE_SYMBOL BUILDING_GKR_MEDIATOR)
set_target_properties(gkr_mediator PROPERTIES DEFINE_SYMBOL GKR_COMM_EX_API)

#target_sources(
#    gkr_mediator
#    INTERFACE
#    "gkr/mediator/api.h"
#)
#target_sources(
#    gkr_mediator
#    PRIVATE
#    "gkr/mediator/libmain.cpp"
#)
