add_library(gkr_mediator SHARED)

target_link_libraries(gkr_mediator PRIVATE gkr_core gkr_data gkr_comm)
target_link_libraries(gkr_mediator PRIVATE websockets_shared)
add_dependencies(gkr_mediator websockets_shared)

target_include_directories(gkr_mediator PUBLIC ".")

set_target_properties(gkr_mediator PROPERTIES DEFINE_SYMBOL BUILDING_GKR_MEDIATOR)

target_sources(
    gkr_mediator
    INTERFACE
    "gkr/mediator/api.h"
    )
target_sources(
    gkr_mediator
    PRIVATE
    "gkr/mediator/libmain.cpp"
    )
