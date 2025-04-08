#pragma once

extern void* gkr_comm_log_instance;

extern void gkr_comm_log_init();

enum
{
    COMM_LOG_FACILITY_LIBRARY,
    COMM_LOG_FACILITY_LWS,
};

#ifndef LOG_FACILITY
#define LOG_FACILITY    COMM_LOG_FACILITY_LIBRARY
#endif
#define LOG_CHANNEL     gkr_comm_log_instance

#include <gkr/log/defs/generic_cdefs.hpp>

