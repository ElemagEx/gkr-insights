#include <gkr/capi/defs.h>

#include <gkr/capi/net/lib.h>
#include <gkr/capi/comm/registry.h>

#include <gkr/capi/log/logging.h>
#include <gkr/capi/log/defs/generic.h>
#include <gkr/capi/log/consumers/app_console_consumer.h>

#include <stdio.h>

int init(int argc, int argv)
{
    struct gkr_log_name_id_pair severities_infos[] = LOG_SEVERITIES_INFOS;
    struct gkr_log_app_console_consumer_callbacks console_callbacks = {{NULL,NULL,NULL,NULL},NULL};

    gkr_net_lib_startup();

    gkr_log_init(NULL, 32, 1024, severities_infos, NULL);
    gkr_log_add_app_console_consumer(NULL, NULL, &console_callbacks, gkr_log_appConsoleWriteMethod_puts, 0);

    gkr_comm_providers_registry_init(1);
    gkr_comm_register_client_provider(NULL);

    return 1;
}

void done()
{
    gkr_comm_providers_registry_done();

    gkr_log_done();

    gkr_net_lib_shutdown();
}

int main(int argc, int argv)
{
    init(argc, argv);
    LOGI("Tester start");

    if(!gkr_comm_providers_start_all())
    {
        LOGF("Tester initialization failed");
    }
    else for(char buf[64]; ; )
    {
        gets_s(buf, sizeof(buf));

        if(*buf == 0) break;
    }
    gkr_comm_providers_stop_all();

    LOGI("Tester finish");
    done();
}
