#include <gkr/stamp.hpp>

#include <thread>
#include <iostream>

extern "C" int do_main(int argc, const char **argv);
extern "C" void end_main();

#include <gkr/log/logging.hpp>
#include <gkr/log/defs/generic_cdefs.hpp>
#include <gkr/log/consumers/app_console_consumer.hpp>

constexpr gkr_log_name_id_pair g_severities_infos[] = {
    {"Fatal"  , LOG_SEVERITY_FATAL  },
    {"Error"  , LOG_SEVERITY_ERROR  },
    {"Warning", LOG_SEVERITY_WARNING},
    {"Info"   , LOG_SEVERITY_INFO   },
    {"Verbose", LOG_SEVERITY_VERBOSE},
    {nullptr  , 0               }
};

static gkr::log::logging s_logging("logger", 32U, 1023U, g_severities_infos);

int main(int argc, const char **argv)
{
    gkr_log_add_consumer(nullptr, std::make_shared<gkr::log::app_console_consumer>(gkr_log_appConsoleWriteMethod_printf));

    int res = 0;

    std::thread thread([argc, argv, &res]()
    {
        LOGI("WebSocket server starting");

        res = do_main(argc, argv);

        LOGI("WebSocket server finished");
    });

    std::string str;
    std::getline(std::cin, str);

    end_main();
    thread.join();

    return res;
}
