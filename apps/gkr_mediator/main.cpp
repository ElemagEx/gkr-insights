#include <gkr/defs.hpp>

#include <gkr/params.hpp>
#include <gkr/net/lib.hpp>
#include <gkr/comm/registry.hpp>
#include <gkr/comm/constants.hpp>
#include <gkr/comm/log_receiver.hpp>

#include <gkr/log/logging.hpp>
#include <gkr/log/defs/generic_cdefs.hpp>
#include <gkr/log/consumers/app_console_consumer.hpp>

#include <string>
#include <iostream>

constexpr gkr_log_name_id_pair g_severities_infos[] = LOG_SEVERITIES_INFOS;

static gkr::net ::lib      s_lib;
static gkr::log ::logging  s_logging("logger", 32U, 1023U, g_severities_infos);
static gkr::comm::registry s_registry(false);

static gkr::comm::log_receiver s_log_receiver;

static gkr::singlethreaded_params s_params;

void load_config()
{
    auto lock = s_params.get_writer_lock();

    s_params.reserve(1024, 1024);

    int root = s_params.add_object("ws-log-receiver");

    s_params.set_value(COMM_PARAM_BRIDGE_RECV_QUEUE_INIT_ELEMENT_COUNT, 50  , root);
    s_params.set_value(COMM_PARAM_BRIDGE_RECV_QUEUE_INIT_ELEMENT_SIZE , 1024, root);
    s_params.set_value(COMM_PARAM_BRIDGE_RECV_QUEUE_RESERVE_FACTOR    , 1.2f, root);
}
void save_config()
{
    s_params.clear();
}

bool init(int argc, int argv)
{
    load_config();

    gkr_log_add_consumer(nullptr, std::make_shared<gkr::log::app_console_consumer>(gkr_log_appConsoleWriteMethod_puts));

    LOGV("Mediator starting...");

    s_registry.register_provider(nullptr);

    s_log_receiver.run();

    LOGI("Mediator started");

    return s_registry.start_all_providers();
}

int done()
{
    LOGV("Mediator stopping...");

    s_log_receiver.quit();

    LOGV("Mediator stopped");

    save_config();
    return 0;
}

int main(int argc, int argv)
{
    if(!init(argc, argv))
    {
        LOGF("Mediator initialization failed");
    }
    else for(std::string line; ; )
    {
        std::getline(std::cin, line);

        if(line.empty()) break;
    }
    return done();
}
