add_executable(gkr_tester "main.c")

target_link_libraries(gkr_tester PRIVATE gkr_inline gkr_core gkr_log gkr_comm)

#target_sources(
#    gkr_mediator
#    INTERFACE
#    "a.hpp"
#    "a.cpp"
#)
