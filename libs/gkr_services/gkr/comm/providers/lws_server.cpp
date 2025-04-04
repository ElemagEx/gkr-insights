#include <gkr/defs.hpp>

#include <gkr/comm/providers/lws_server.hpp>
#include <gkr/comm/providers/lws_log_sink.hpp>

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

server* server::create()
{
    return new server;
}

server::server() noexcept
{
}

server::~server() noexcept
{
}

void server::release()
{
    delete this;
}

std::shared_ptr<bridge> server::create_bridge(const char* service_name, const char* transport, end_point* ep)
{
    Check_Arg_NotNull(service_name, {});
    Check_Arg_NotNull(ep          , {});

    service* s = nullptr;

    if(s == nullptr)
    {
        Check_Recovery("Unknown/unsupported service name");
        return nullptr;
    }
    return std::make_shared<bridge>(ep, s);
}

void server::get_context_info(unsigned long long& options)
{
    options = LWS_SERVER_OPTION_HTTP_HEADERS_SECURITY_BEST_PRACTICES_ENFORCE;
}

bool server::get_server_info(int& port, const struct lws_http_mount*& mount)
{
    static const struct lws_http_mount http_mount
    {
        /* .mount_next */            nullptr,          /* linked-list "next" */
        /* .mountpoint */            "/",              /* mountpoint URL */
        /* .origin */                "./mount-origin", /* serve from dir */
        /* .def */                   "index.html",     /* default filename */
        /* .protocol */              nullptr,
        /* .cgienv */                nullptr,
        /* .extra_mimetypes */       nullptr,
        /* .interpret */             nullptr,
        /* .cgi_timeout */           0,
        /* .cache_max_age */         0,
        /* .auth_mask */             0,
        /* .cache_reusable */        0,
        /* .cache_revalidate */      0,
        /* .cache_intermediaries */  0,
        /* .origin_protocol */       LWSMPRO_FILE,     /* files in a dir */
        /* .mountpoint_len */        1,                /* char count */
        /* .basic_auth_login_file */ nullptr,
    };
    port  = 7681;
    mount = &http_mount;
    return true;
}

}
}
}
}
