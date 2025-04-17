#include <gkr/defs.hpp>

#include <gkr/comm/providers/lws_context.hpp>
#include <gkr/comm/providers/lws_instance.hpp>
#include <gkr/comm/providers/lws_protocol.hpp>

#include <gkr/comm/constants.hpp>
#include <gkr/comm/bridge.hpp>

#include <gkr/comm/log.hxx>

#include <gkr/diagnostics.hpp>

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

static void log_callback(int level, const char *line)
{
    int severity;
    if(level == 0)
    {
        severity = LOG_SEVERITY_FATAL;
    }
    else if(level & LLL_ERR   ) severity = LOG_SEVERITY_ERROR;
    else if(level & LLL_WARN  ) severity = LOG_SEVERITY_WARNING;
    else if(level & LLL_INFO  ) severity = LOG_SEVERITY_INFO;
    else if(level & LLL_NOTICE) severity = LOG_SEVERITY_VERBOSE;
    else if(level & LLL_DEBUG ) severity = LOG_SEVERITY_DEBUG;
    else
    {
        severity = LOG_SEVERITY_TRACE;
    }

    const char* pos = std::strchr(line, ']');
    if(pos != nullptr) line = pos + 2;

    gkr_log_simple_message(gkr_comm_log_instance, severity, COMM_LOG_FACILITY_LWS, line);
}

context* context::create(const params* parameters, std::size_t root)
{
    return new context(parameters, root);
}

context::context(const params* parameters, std::size_t root) : m_params(parameters), m_root(root)
{
    m_protocols.reserve(4);
    m_protocols.push_back(LWS_PROTOCOL_LIST_TERM);
}

context::~context()
{
}

int context::thread_init()
{
    Assert_Check(m_protocols.size() >= 2);

    struct lws_context_creation_info info {};

    if(!setup(info)) return -1;

    info.user      = this;
    info.protocols = m_protocols.data();

    m_context = lws_create_context(&info);

    if(m_context == nullptr) return -1;

    for(std::size_t index = 0; index < m_protocols.size() - 1; ++index)
    {
        static_cast<protocol*>(m_protocols[index].user)->m_context = m_context;
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
    if(m_context == nullptr) return;

    Assert_Check(m_protocols.size() >= 1);

    lws_context_destroy(m_context);
    m_context = nullptr;

    for(std::size_t index = m_protocols.size() - 1; index-- > 0; )
    {
        delete static_cast<protocol*>(m_protocols[index].user);
    }
    m_protocols.clear();
    m_protocols.push_back(LWS_PROTOCOL_LIST_TERM);
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

    lws_log_emit_t callback = (gkr_comm_log_instance == nullptr)
        ? nullptr
        : log_callback
        ;
    lws_set_log_level(0xFFF, callback);// LLL_USER

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

std::shared_ptr<bridge> context::create_bridge(end_point* ep, const char* service_name, const char* transport, int port)
{
    Check_Arg_NotNull(ep          , {});
    Check_Arg_NotNull(service_name, {});
    Check_Arg_NotNull(transport   , {});

    Check_Arg_IsValid((port >= -1) && (port <= 65535), {});

    Check_ValidState(!is_running(), {});

    protocol* proto = nullptr;

    if(!std::strcmp(service_name, COMM_SERVICE_NAME_LOG_UPSTREAM_CLIENT))
    {
        if(!std::strcmp(transport, COMM_TRANSPORT_WEB_SOCKET_PLAIN))
        {
            proto = add_protocol(new client_protocol("gkr-log-pipe", service::FLAG_CAN_SEND_DATA));
        }
        else if(!std::strcmp(transport, COMM_TRANSPORT_WEB_SOCKET_SECURE))
        {
            proto = add_protocol(new client_protocol("gkr-log-pipe", service::FLAG_CAN_SEND_DATA | service::FLAG_SECURE));
        }
        else
        {
            LOGE_("libwebsocket comm provider does not support transport: %s", transport);
        }
    }
    else if(!std::strcmp(service_name, COMM_SERVICE_NAME_LOG_UPSTREAM_SERVER))
    {
        if(!std::strcmp(transport, COMM_TRANSPORT_WEB_SOCKET_PLAIN))
        {
            proto = add_protocol(new server_protocol("gkr-log-sink", service::FLAG_CAN_RECEIVE_DATA, port));
        }
        else if(!std::strcmp(transport, COMM_TRANSPORT_WEB_SOCKET_SECURE))
        {
            proto = add_protocol(new server_protocol("gkr-log-sink", service::FLAG_CAN_RECEIVE_DATA | service::FLAG_SECURE, port));
        }
        else
        {
            LOGE_("libwebsocket comm provider does not support transport: %s", transport);
        }
    }
    else
    {
        LOGE_("libwebsocket comm provider does not recongnize service name: %s", service_name);
    }
    if(proto == nullptr) return nullptr;

    bridge_ptr_t bridge_ptr = std::make_shared<bridge>(ep, proto);

    proto->m_bridge = bridge_ptr;

    return bridge_ptr;
}

protocol* context::add_protocol(protocol* p, bool infront)
{
    Assert_Check(!m_protocols.empty());

    if(infront) m_protocols.insert(m_protocols.begin(), LWS_PROTOCOL_LIST_TERM);

    struct lws_protocols& proto = infront
        ? m_protocols.front()
        : m_protocols.back();

    proto.user     = p;
    proto.callback = static_cast<lws_callback_function *>(static_cast<protocol*>(proto.user)->get_callback());
    proto.name     = static_cast<protocol*>(proto.user)->get_name();
    proto.id       = static_cast<protocol*>(proto.user)->get_info(proto.rx_buffer_size, proto.tx_packet_size);

    proto.per_session_data_size = sizeof(instance);

    if(!infront) m_protocols.push_back(LWS_PROTOCOL_LIST_TERM);

    return static_cast<protocol*>(proto.user);
}

bool context::setup(struct lws_context_creation_info& info)
{
    bool is_secure = false;
    info.port = CONTEXT_PORT_NO_LISTEN;

    for(std::size_t index = 0; index < m_protocols.size() - 1; ++index)
    {
        protocol* proto = static_cast<protocol*>(m_protocols[index].user);

        if(proto->is_client())
        {
            info.options |= LWS_SERVER_OPTION_DO_SSL_GLOBAL_INIT;
        }
        if(proto->is_server())
        {
            if(info.port != CONTEXT_PORT_NO_LISTEN)
            {
                if((info.port != proto->get_listen_port()) || (is_secure != proto->is_secure()))
                {
                    LOGE("Currently libwebsocket comm provider does not support multiple virtual hosts");
                    return false;
                }
            }
            info.port = proto->get_listen_port();

            info.options |= LWS_SERVER_OPTION_HTTP_HEADERS_SECURITY_BEST_PRACTICES_ENFORCE;
        }
    }
    if(info.port != CONTEXT_PORT_NO_LISTEN)
    {
        add_protocol(new dummy_protocol(), true);

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
        info.mounts = &http_mount;
    }
    return true;
}

}
}
}
}
