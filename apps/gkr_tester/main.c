#include <gkr/capi/defs.h>

#include <gkr/capi/params.h>

#include <gkr/capi/net/lib.h>
#include <gkr/capi/comm/registry.h>
#include <gkr/capi/comm/constants.h>
#include <gkr/capi/comm/log_upstream_log_consumer.h>

#include <gkr/capi/log/logging.h>
#include <gkr/capi/log/defs/generic.h>
#include <gkr/capi/log/consumers/app_console_consumer.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

static struct gkr_params* params = NULL;

void load_config()
{
    int root;

    params = gkr_params_create(false, malloc, free);
    gkr_params_reserve(params, 1024, 1024);

    root = gkr_params_add_object(params, "lws", 0);

    root = gkr_params_add_object(params, "ws-log-upstream", 0);

    gkr_params_set_value_string (params, COMM_PARAM_PROTOCOL_TRANSPORT    , COMM_TRANSPORT_WEB_SOCKET_PLAIN, root, false);
    gkr_params_set_value_string (params, COMM_PARAM_PROTOCOL_FORMAT       , "binary", root, false);
    gkr_params_set_value_integer(params, COMM_PARAM_PROTOCOL_VERSION      , 0, root, false);
    gkr_params_set_value_string (params, COMM_PARAM_PROTOCOL_REMOTE_SERVER, "192.168.0.110", root, false);

    gkr_params_set_value_integer(params, COMM_PARAM_BRIDGE_SEND_QUEUE_INIT_ELEMENT_COUNT, 50  , root, false);
    gkr_params_set_value_integer(params, COMM_PARAM_BRIDGE_SEND_QUEUE_INIT_ELEMENT_SIZE , 1024, root, false);
    gkr_params_set_value_double (params, COMM_PARAM_BRIDGE_SEND_QUEUE_RESERVE_FACTOR    , 1.2f, root, false);

}
void save_config()
{
    gkr_params_destroy(params);
    params = NULL;
}

int init(int argc, const char* argv[])
{
    struct gkr_log_name_id_pair severities_infos[] = LOG_SEVERITIES_INFOS;
    struct gkr_log_name_id_pair facilities_infos[] = {{"Tester", 0}, {NULL, 0}};
    int id;

    load_config();

    gkr_net_lib_startup();

    gkr_log_init(NULL, 32, 1024, severities_infos, facilities_infos, true);
    id = gkr_log_add_app_console_consumer(NULL, NULL, NULL, gkr_log_appConsoleWriteMethod_puts, 0);
    LOGI("GKR Tester v0.1");

    LOGV("Tester starting...");

    gkr_comm_providers_registry_init(true);
    gkr_log_add_consumer_by_id(gkr_log_get_channel(COMM_LOG_CHANNEL_NAME), id);

    gkr_comm_register_provider_ex(COMM_PROVIDER_NAME_LIBWEBSOCKET, params, gkr_params_find_node(params, "lws"));

    gkr_comm_add_upstream_log_consumer/*_ex*/(
        NULL,
        NULL,
        NULL,
//        "wss://localhost/binary/v0",
//        NULL,
//        NULL,
        params,
        gkr_params_find_node(params, "ws-log-upstream"));

    gkr_comm_providers_start_all();
    LOGI("Tester started");
    return 1;
}

int done()
{
    LOGV("Tester stopping...");

    gkr_comm_providers_registry_done();

    LOGI("Tester stopped");

    gkr_log_done();

    gkr_net_lib_shutdown();

    save_config();
    return 0;
}

int main(int argc, const char** argv)
{
    if(!init(argc, argv))
    {
        LOGF("Tester initialization failed");
    }
    else for(char buf[64]; ; )
    {
        LOGI("Tester running...");

        gets_s(buf, sizeof(buf));

        if(*buf == 0) break;
    }
    return done();
}
