add_executable(gkr_mediator "main.cpp")

target_link_libraries(gkr_mediator PRIVATE gkr_inline gkr_core gkr_log gkr_comm)
