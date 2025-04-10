#include <gkr/defs.hpp>

#include <gkr/comm/providers/lws_protocol.hpp>

#include <gkr/url.hpp>
#include <gkr/diagnostics.hpp>
#include <gkr/log/defs/generic_cdefs.hpp>

#include <libwebsockets.h>

namespace gkr
{
namespace comm
{
namespace providers
{
namespace libwebsocket
{

struct per_vhost_data
{
    const
    struct lws_protocols* proto;
    struct lws_context*   context;
    struct lws_vhost*     vhost;
};
//struct per_vhost_data* vhd = (struct per_vhost_data*)lws_protocol_vh_priv_get(lws_get_vhost(wsi), lws_get_protocol(wsi));
//vhd = (struct per_vhost_data*)lws_protocol_vh_priv_zalloc(lws_get_vhost(wsi), lws_get_protocol(wsi), sizeof(struct per_vhost_data));
//vhd->context  = lws_get_context(wsi);
//vhd->vhost    = lws_get_vhost(wsi);
//vhd->protocol = lws_get_protocol(wsi);
struct per_session_data
{
    struct lws* wsi;
};

int dummy_callback(struct lws* wsi, enum lws_callback_reasons reason, void* user, void* in, size_t len)
{
    struct per_session_data* psd = (struct per_session_data*)user;

    const struct lws_protocols* proto = lws_get_protocol(wsi);

    if((proto != nullptr) && (proto->user != nullptr))
    {
        static_cast<dummy_protocol*>(proto->user)->on_other_reason(reason, in, len);
    }
    return lws_callback_http_dummy(wsi, reason, user, in, len);
}
void* dummy_protocol::get_callback() noexcept
{
    return dummy_callback;
}
int dummy_protocol::get_listen_port() noexcept
{
    return 0;
}
const char* dummy_protocol::get_name() noexcept
{
    return "http";
}
unsigned dummy_protocol::get_info(std::size_t& ps_size, std::size_t& rx_size, std::size_t& tx_size)
{
    ps_size = 0;
    rx_size = 0;
    tx_size = 0;
    return 0;
}
void dummy_protocol::on_other_reason(int reason, const void* data, std::size_t size)
{
}
bool dummy_protocol::can_connect()
{
    return false;
}
bool dummy_protocol::can_listen()
{
    return false;
}
void dummy_protocol::connect()
{
}
bool dummy_protocol::listen()
{
    return false;
}
void dummy_protocol::close()
{
}
void dummy_protocol::on_data_sent()
{
}
void dummy_protocol::on_error()
{
}

int server_callback(struct lws* wsi, enum lws_callback_reasons reason, void* user, void* in, std::size_t len)
{
    struct per_session_data* psd = (struct per_session_data*)user;

    const struct lws_protocols* proto = lws_get_protocol(wsi);

    if(proto == nullptr)
    {
    }
    else switch(reason)
    {
        case LWS_CALLBACK_PROTOCOL_INIT:
            static_cast<server_protocol*>(proto->user)->on_init();
            break;

        case LWS_CALLBACK_PROTOCOL_DESTROY:
            static_cast<server_protocol*>(proto->user)->on_done();
            break;

        case LWS_CALLBACK_ESTABLISHED:
            static_cast<server_protocol*>(proto->user)->on_connection_opened();
            break;

        case LWS_CALLBACK_CLOSED:
            static_cast<server_protocol*>(proto->user)->on_connection_closed();
            break;

        case LWS_CALLBACK_SERVER_WRITEABLE:
            static_cast<server_protocol*>(proto->user)->on_connection_writeable();
            break;

        case LWS_CALLBACK_RECEIVE:
            static_cast<server_protocol*>(proto->user)->on_connection_received_data(in, len);
            break;

        case LWS_CALLBACK_RECEIVE_PONG:
            static_cast<server_protocol*>(proto->user)->on_connection_pong();
            break;

        default:
            static_cast<server_protocol*>(proto->user)->on_other_reason(reason, in, len);
            break;
    }
    return 0;
}

void* server_protocol::get_callback() noexcept
{
    return server_callback;
}

int client_callback(struct lws* wsi, enum lws_callback_reasons reason, void* user, void* in, std::size_t len)
{
    struct per_session_data* psd = (struct per_session_data*)user;

    const struct lws_protocols* proto = lws_get_protocol(wsi);

    if(proto == nullptr) return 0;

    switch(reason)
    {
        case LWS_CALLBACK_PROTOCOL_INIT:
            static_cast<client_protocol*>(proto->user)->on_init();
            break;

        case LWS_CALLBACK_PROTOCOL_DESTROY:
            static_cast<client_protocol*>(proto->user)->on_done();
            break;

        case LWS_CALLBACK_CLIENT_CONNECTION_ERROR:
            static_cast<client_protocol*>(proto->user)->on_connection_error(static_cast<const char*>(in));
            break;

        case LWS_CALLBACK_CLIENT_ESTABLISHED:
            static_cast<client_protocol*>(proto->user)->on_connection_opened();
            break;

        case LWS_CALLBACK_CLIENT_CLOSED:
            static_cast<client_protocol*>(proto->user)->on_connection_closed();
            break;

        case LWS_CALLBACK_CLIENT_WRITEABLE:
            static_cast<client_protocol*>(proto->user)->on_connection_writeable();
            break;

        case LWS_CALLBACK_CLIENT_RECEIVE:
            static_cast<client_protocol*>(proto->user)->on_connection_received_data(in, len);
            break;

        case LWS_CALLBACK_CLIENT_RECEIVE_PONG:
            static_cast<client_protocol*>(proto->user)->on_connection_pong();
            break;

        default:
            static_cast<client_protocol*>(proto->user)->on_other_reason(reason, in, len);
            break;
    }
    return 0;
}

void* client_protocol::get_callback() noexcept
{
    return client_callback;
}

int client_protocol::get_listen_port() noexcept
{
    return CONTEXT_PORT_NO_LISTEN;
}

bool client_protocol::connect(const char* url, const char* protocol)
{
    Check_Arg_NotNull(url);

    if(m_connect_url.is_valid()) return false;

    m_connect_url.decompose(url);

    if(!m_connect_url.is_valid()) return false;

    struct lws_client_connect_info info;
    std::memset(&info, 0, sizeof(info));

    int ssl_connection;

    if     (!std::strcmp(m_connect_url.parts().scheme, "wss")) {ssl_connection = LCCSCF_USE_SSL; }
    else if(!std::strcmp(m_connect_url.parts().scheme, "ws" )) {ssl_connection = 0; }
    else
    {
        m_connect_url.clear();
        return false;
    }
    info.context               = get_parent_context();
    info.port                  = m_connect_url.parts().port;
    info.address               = m_connect_url.parts().host;
    info.path                  = m_connect_url.parts().path;
    info.host                  = info.address;
    info.origin                = info.address;
    info.ssl_connection        = ssl_connection;
    info.protocol              = protocol;
    info.local_protocol_name   = get_name();
//  info.retry_and_idle_policy = &retry;

    return true;
}

}
}
}
}
