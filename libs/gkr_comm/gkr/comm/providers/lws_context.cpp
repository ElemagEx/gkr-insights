#include <gkr/defs.hpp>

#include <gkr/comm/providers/lws_context.hpp>
#include <gkr/comm/providers/lws_protocol.hpp>
#include <gkr/comm/providers/lws_log_pipe.hpp>
#include <gkr/comm/providers/lws_log_sink.hpp>

#include <gkr/comm/constants.hpp>
#include <gkr/comm/bridge.hpp>

#include <gkr/diagnostics.hpp>
#include <gkr/log/defs/generic_cdefs.hpp>

#include <libwebsockets.h>

#include <cstring>
#include <atomic>

namespace gkr
{
namespace comm
{
namespace providers
{
namespace libwebsocket
{

context* context::create()
{
    return new context();
}

context::context() noexcept
{
    m_protocols.reserve(4);
    m_protocols.push_back(LWS_PROTOCOL_LIST_TERM);
}

context::~context() noexcept
{
}

int context::thread_init()
{
    if(m_protocols.size() < 2) return -1;

    struct lws_context_creation_info info;

    std::memset(&info, 0, sizeof(info));

    info.user      = this;
    info.protocols = m_protocols.data();

    get_context_info(info.options);
    get_server_info(info.port, info.mounts);

    m_context = lws_create_context(&info);

    if(m_context == nullptr) return -1;

    for(std::size_t index = 0; index < m_protocols.size() - 1; ++index)
    {
        static_cast<protocol*>(m_protocols[index].user)->set_parent_context(m_context);
    }
    return +1;
}

void context::thread_loop()
{
    for(m_running = true; m_running; )
    {
        if(0 > lws_service(m_context, 0))
        {
            m_running = false;
        }
    }
}

void context::thread_done()
{
    if(m_context != nullptr)
    {
        lws_context_destroy(m_context);
        m_context = nullptr;
    }
}

void context::release()
{
    delete this;
}

const char* context::get_name()
{
    return "libwebsocket";
}

bool context::start()
{
    Check_ValidState(!m_thread.joinable(), false);

    lws_set_log_level(LLL_USER | LLL_ERR | LLL_WARN | LLL_NOTICE, nullptr);

    std::atomic_int initialized { 0 };

    m_thread = std::thread([this, &initialized] () {
        int result = thread_init();
        initialized.store(result, std::memory_order_release);
        if(result != -1) thread_loop();
        thread_done();
    });
    for( ; ; std::this_thread::yield())
    {
        int result = initialized.load(std::memory_order_relaxed);

        if(result ==  0) continue;
        if(result != -1) return true;

        m_thread.join();
        return false;
    }
}

void context::stop()
{
    if(!m_thread.joinable()) return;

    m_running = false;

    lws_cancel_service(m_context);

    m_thread.join();
}

std::shared_ptr<bridge> context::create_bridge(const char* service_name, const char* transport, end_point* ep)
{
    Check_Arg_NotNull(service_name, {});
    Check_Arg_NotNull(ep          , {});

    service* s = nullptr;

    if(*service_name == 0)
    {
    }
    else if(!std::strcmp(service_name, COMM_SERVICE_NAME_LOG_UPSTREAM_CLIENT))
    {
        protocol* p = find_protocol(log_pipe::NAME);

        if(p == nullptr)
        {
            p = new log_pipe();
            add_protocol(p);
            s = p;
        }
    }
    else if(!std::strcmp(service_name, COMM_SERVICE_NAME_LOG_UPSTREAM_SERVER))
    {
        protocol* p = find_protocol(log_sink::NAME);

        if(p == nullptr)
        {
            p = new log_sink();
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

protocol* context::find_protocol(const char* name)
{
    Check_Arg_NotNull(name);

    std::size_t count = m_protocols.size();

    if(count-- == 1) return nullptr;

    for(std::size_t index = 0; index < count; ++index)
    {
        protocol* p = static_cast<protocol*>(m_protocols[index].user);

        if(!std::strcmp(name, p->get_name()))
        {
            return p;
        }
    }
    return nullptr;
}

void context::add_protocol(protocol* p)
{
    Assert_Check(!m_protocols.empty());

    struct lws_protocols& proto = m_protocols.back();

    proto.user     = p;
    proto.callback = static_cast<lws_callback_function *>(static_cast<protocol*>(proto.user)->get_callback());
    proto.name     = static_cast<protocol*>(proto.user)->get_name();
    proto.id       = static_cast<protocol*>(proto.user)->get_info(
        proto.per_session_data_size,
        proto.rx_buffer_size,
        proto.tx_packet_size);

    m_protocols.push_back(LWS_PROTOCOL_LIST_TERM);
}

void context::get_context_info(unsigned long long& options)
{
    for(std::size_t index = 0; index < m_protocols.size() - 1; ++index)
    {
        if(static_cast<protocol*>(m_protocols[index].user)->can_connect())
        {
            options |= LWS_SERVER_OPTION_DO_SSL_GLOBAL_INIT;
        }
        if(static_cast<protocol*>(m_protocols[index].user)->can_listen())
        {
            options = LWS_SERVER_OPTION_HTTP_HEADERS_SECURITY_BEST_PRACTICES_ENFORCE;
        }
    }
}

bool context::get_server_info(int& port, const struct lws_http_mount*& mount)
{
    //SERVER
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
    Assert_Check(m_protocols.size() == 2);//???
    for(std::size_t index = 0; index < m_protocols.size() - 1; ++index)
    {
        if(static_cast<protocol*>(m_protocols[index].user)->can_connect())
        {
            //???
            port  = CONTEXT_PORT_NO_LISTEN;
            mount = nullptr;
        }
        if(static_cast<protocol*>(m_protocols[index].user)->can_listen())
        {
            //???
            port  = 7681;
            mount = &http_mount;
        }
    }
    return true;
}

}
}
}
}
