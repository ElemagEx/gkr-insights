#include <gkr/defs.hpp>

#include "lws_server.hpp"
#include "lws_log_sink.hpp"

#include <gkr/diagnostics.hpp>

#include <libwebsockets.h>

namespace gkr
{
namespace mediator
{
namespace lws
{

Server::Server() noexcept
{
}

Server::~Server() noexcept
{
}

void Server::get_context_info(unsigned& protocols, unsigned long long& options)
{
    protocols = 2;
    options   = LWS_SERVER_OPTION_HTTP_HEADERS_SECURITY_BEST_PRACTICES_ENFORCE;
}

bool Server::get_server_info(int& port, unsigned& http_dummy_protocol_index, const struct lws_http_mount*& mount)
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
    port = 7681;
    http_dummy_protocol_index = 0;
    mount = &http_mount;
    return true;
}

Protocol* Server::create_protocol(unsigned index)
{
    switch(index)
    {
        case 1: return new gkr::mediator::lws::LogSink();
    }
    Check_Failure(nullptr);
}

}
}
}
