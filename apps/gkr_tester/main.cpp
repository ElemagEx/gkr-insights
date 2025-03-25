#include <gkr/defs.hpp>

#include <gkr/net/lib.hpp>
#include <gkr/comm/storage.hpp>
#include <gkr/pipes/client_factory.hpp>

#include <gkr/log/logging.hpp>
#include <gkr/log/defs/generic_cdefs.hpp>
#include <gkr/log/consumers/app_console_consumer.hpp>

#include <string>
#include <iostream>

constexpr gkr_log_name_id_pair g_severities_infos[] = LOG_SEVERITIES_INFOS;

static gkr::log::logging s_logging("logger", 32U, 1023U, g_severities_infos);
static gkr::net::lib;

static gkr::providers::storage pipes_providers(true);

int main(int argc, int argv)
{
    gkr_log_add_consumer(nullptr, std::make_shared<gkr::log::app_console_consumer>(gkr_log_appConsoleWriteMethod_puts));

    LOGI("Tester start");

    pipes_providers.add_provider(gkr::providers::create_client_provider());

    if(!pipes_providers.start())
    {
        LOGF("Tester initialization failed");
    }
    else for(std::string line; ; )
    {
        std::getline(std::cin, line);

        if(line.empty())
        {
            pipes_providers.stop();
            break;
        }
    }
    LOGI("Tester finish");
}
