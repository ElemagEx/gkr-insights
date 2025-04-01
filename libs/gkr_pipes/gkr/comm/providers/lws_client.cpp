#include <gkr/defs.hpp>

#include <gkr/comm/providers/lws_client.hpp>
#include <gkr/comm/providers/lws_log_pipe.hpp>

#include <gkr/comm/constants.hpp>
#include <gkr/comm/bridge.hpp>

#include <gkr/diagnostics.hpp>

#include <libwebsockets.h>

namespace gkr
{
namespace comm
{
namespace providers
{
namespace libwebsocket
{

client* client::create()
{
    return new client();
}

client::client() noexcept
{
}

client::~client() noexcept
{
}

void client::release()
{
    delete this;
}

std::shared_ptr<bridge> client::create_bridge(const char* service_name, end_point* ep)
{
    Check_Arg_NotNull(service_name, {});
    Check_Arg_NotNull(ep          , {});

    service* s = nullptr;

    if(!std::strcmp(service_name, COMM_SERVICE_NAME_LOG_CONSUMER))
    {
        protocol* p = find_protocol(log_pipe::NAME);

        if(p == nullptr)
        {
            p = new log_pipe();
            add_protocol(p);
            s = p;
        }
    }
    if(s == nullptr)
    {
        Check_Recovery("Unknown/unsupported service name");
        return nullptr;
    }
    return std::make_shared<bridge>(ep, s);
}

void client::get_context_info(unsigned long long& options)
{
    options = LWS_SERVER_OPTION_DO_SSL_GLOBAL_INIT;
}

bool client::get_server_info(int& port, const struct lws_http_mount*& mount)
{
    port  = CONTEXT_PORT_NO_LISTEN;
    mount = nullptr;
    return true;
}

}
}
}
}
