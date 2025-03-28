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

    LOGV("Tester starting...");

    gkr_comm_providers_registry_init(1);
    gkr_comm_client_register_provider(NULL);
    gkr_comm_providers_start_all();

    LOGI("Tester started");

    return 1;
}

void done()
{
    LOGV("Tester stopping...");

    gkr_comm_providers_registry_done();

    LOGV("Tester stopped");

    gkr_log_done();

    gkr_net_lib_shutdown();
}

int main(int argc, int argv)
{
    if(!init(argc, argv))
    {
        LOGF("Tester initialization failed");
    }
    else for(char buf[64]; ; )
    {
        LOGI("Tester Running...");

        gets_s(buf, sizeof(buf));

        if(*buf == 0) break;
    }
    done();
}
