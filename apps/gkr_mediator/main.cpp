#include "lws_context.hpp"

#include "lws_server.hpp"

#include <gkr/log/logging.hpp>
#include <gkr/log/defs/generic_cdefs.hpp>
#include <gkr/log/consumers/app_console_consumer.hpp>

#include <string>
#include <iostream>

constexpr gkr_log_name_id_pair g_severities_infos[] = {
    {"Fatal"  , LOG_SEVERITY_FATAL  },
    {"Error"  , LOG_SEVERITY_ERROR  },
    {"Warning", LOG_SEVERITY_WARNING},
    {"Info"   , LOG_SEVERITY_INFO   },
    {"Verbose", LOG_SEVERITY_VERBOSE},
    {nullptr  , 0                   }
};

static gkr::log::logging s_logging("logger", 32U, 1023U, g_severities_infos);

int main(int argc, int argv)
{
    gkr_log_add_consumer(nullptr, std::make_shared<gkr::log::app_console_consumer>(gkr_log_appConsoleWriteMethod_printf));

    LOGI("Server start");

    gkr::mediator::lws::Server server;

    if(!server.run())
    {
        LOGF("Server initialization failed");
    }
    else for(std::string line; ; )
    {
        std::getline(std::cin, line);

        if(line.empty())
        {
            server.stop();
            break;
        }

    }
    LOGI("Server finish");
}
