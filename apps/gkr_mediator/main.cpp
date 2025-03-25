#include <gkr/defs.hpp>

#include <gkr/net/lib.hpp>
#include <gkr/comm/storage.hpp>
#include <gkr/services/server_factory.hpp>

#include <gkr/log/logging.hpp>
#include <gkr/log/defs/generic_cdefs.hpp>
#include <gkr/log/consumers/app_console_consumer.hpp>

#include <string>
#include <iostream>

constexpr gkr_log_name_id_pair g_severities_infos[] = LOG_SEVERITIES_INFOS;

static gkr::log::logging s_logging("logger", 32U, 1023U, g_severities_infos);
static gkr::net::lib;

static gkr::providers::storage services_providers(false);

int main(int argc, int argv)
{
    gkr_log_add_consumer(nullptr, std::make_shared<gkr::log::app_console_consumer>(gkr_log_appConsoleWriteMethod_puts));

    LOGI("Server start");

    services_providers.add_provider(gkr::providers::create_server_provider());

    if(!services_providers.start())
    {
        LOGF("Server initialization failed");
    }
    else for(std::string line; ; )
    {
        std::getline(std::cin, line);

        if(line.empty())
        {
            services_providers.stop();
            break;
        }
    }
    LOGI("Server finish");
}
