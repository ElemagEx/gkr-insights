#include <gkr/defs.hpp>

#include <gkr/net/lib.hpp>
#include <gkr/comm/registry.hpp>

#include <gkr/log/logging.hpp>
#include <gkr/log/defs/generic_cdefs.hpp>
#include <gkr/log/consumers/app_console_consumer.hpp>

#include <string>
#include <iostream>

constexpr gkr_log_name_id_pair g_severities_infos[] = LOG_SEVERITIES_INFOS;

static gkr::net ::lib      s_lib;
static gkr::log ::logging  s_logging("logger", 32U, 1023U, g_severities_infos);
static gkr::comm::registry s_registry(false);

int main(int argc, int argv)
{
    gkr_log_add_consumer(nullptr, std::make_shared<gkr::log::app_console_consumer>(gkr_log_appConsoleWriteMethod_puts));

    LOGI("Server start");

    s_registry.server_register_provider(nullptr);

    if(!s_registry.start_all_providers())
    {
        LOGF("Server initialization failed");
    }
    else for(std::string line; ; )
    {
        std::getline(std::cin, line);

        if(line.empty()) break;
    }
    s_registry.stop_all_providers();

    LOGI("Server finish");
}
