#include <gkr/defs.hpp>

#include <gkr/comm/log.hxx>

#include <gkr/comm/constants.hpp>

#include <gkr/log/logging.hpp>

void* gkr_comm_log_instance = nullptr;

void gkr_comm_log_init()
{
    gkr_log_name_id_pair severities_infos[] = LOG_SEVERITIES_INFOS;
    gkr_log_name_id_pair facilities_infos[] = {
        {"gkr-comm", COMM_LOG_FACILITY_LIBRARY},
        {"libWS"   , COMM_LOG_FACILITY_LWS    },
        {nullptr  , 0}
    };
    gkr_comm_log_instance = gkr_log_add_channel(COMM_LOG_CHANNEL_NAME, 16, 256, severities_infos, facilities_infos);
}
