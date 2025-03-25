#include <gkr/defs.hpp>

#include <gkr/pipes/lws_client.hpp>
#include <gkr/pipes/lws_log_pipe.hpp>

#include <gkr/diagnostics.hpp>

#include <libwebsockets.h>

namespace gkr
{
namespace providers
{
namespace lws
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

void client::get_context_info(unsigned& protocols, unsigned long long& options)
{
    protocols = 1;
    options   = LWS_SERVER_OPTION_DO_SSL_GLOBAL_INIT;
}

bool client::get_server_info(int& port, const struct lws_http_mount*& mount)
{
    port  = CONTEXT_PORT_NO_LISTEN;
    mount = nullptr;
    return true;
}

protocol* client::create_protocol(unsigned index)
{
    switch(index)
    {
        case 0: return new log_pipe();
    }
    Check_Failure(nullptr);
}

}
}
}
