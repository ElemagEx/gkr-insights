#include <gkr/defs.hpp>

#include <gkr/services/lws_server.hpp>
#include <gkr/services/lws_log_sink.hpp>

#include <gkr/diagnostics.hpp>

#include <libwebsockets.h>

namespace gkr
{
namespace lws
{

server::server() noexcept
{
}

server::~server() noexcept
{
}

void server::get_context_info(unsigned& protocols, unsigned long long& options)
{
    protocols = 2;
    options   = LWS_SERVER_OPTION_HTTP_HEADERS_SECURITY_BEST_PRACTICES_ENFORCE;
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

protocol* server::create_protocol(unsigned index)
{
    switch(index)
    {
        case 0: return new dummy_protocol();
        case 1: return new log_sink();
    }
    Check_Failure(nullptr);
}

}
}
