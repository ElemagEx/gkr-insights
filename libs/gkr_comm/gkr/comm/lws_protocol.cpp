#include <gkr/defs.hpp>

#include <gkr/comm/lws_protocol.hpp>

#include <gkr/url.hpp>
#include <gkr/diagnostics.hpp>
#include <gkr/log/defs/generic_cdefs.hpp>

#include <libwebsockets.h>

namespace gkr
{
namespace providers
{
namespace lws
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

protocol::protocol()
{
}

protocol::~protocol()
{
}

dummy_protocol::dummy_protocol()
{
}

dummy_protocol::~dummy_protocol()
{
}

int dummy_protocol::dummy_callback(struct lws* wsi, enum lws_callback_reasons reason, void* user, void* in, size_t len)
{
    static_cast<dummy_protocol*>(user)->on_other_reason(reason, in, len);

    return lws_callback_http_dummy(wsi, reason, user, in, len);
}

const char* dummy_protocol::get_name()
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

void* dummy_protocol::get_callback()
{
    return dummy_callback;
}

void dummy_protocol::on_other_reason(int reason, const void* data, std::size_t size)
{
}

server_protocol::server_protocol()
{
}

server_protocol::~server_protocol()
{
}

void* server_protocol::get_callback()
{
    return server_callback;
}

int server_protocol::server_callback(struct lws* wsi, enum lws_callback_reasons reason, void* user, void* in, std::size_t len)
{
    struct per_session_data* psd = (struct per_session_data*)user;

    const struct lws_protocols* proto = lws_get_protocol(wsi);

    switch(reason)
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

client_protocol::client_protocol()
{
}

client_protocol::~client_protocol()
{
}

void* client_protocol::get_callback()
{
    return client_callback;
}

bool client_protocol::connect(const char* url, const char* protocol)
{
    Check_Arg_NotNull(url);

    if(m_connect_url.is_valid()) return false;

    m_connect_url.reset(url);

    if(!m_connect_url.is_valid()) return false;

    struct lws_client_connect_info info;
    std::memset(&info, 0, sizeof(info));

    int ssl_connection;

    if     (!std::strcmp(m_connect_url.parts().scheme, "wss")) {ssl_connection = LCCSCF_USE_SSL; }
    else if(!std::strcmp(m_connect_url.parts().scheme, "ws" )) {ssl_connection = 0; }
    else
    {
        m_connect_url.reset();
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

int client_protocol::client_callback(struct lws* wsi, enum lws_callback_reasons reason, void* user, void* in, std::size_t len)
{
    struct per_session_data* psd = (struct per_session_data*)user;

    const struct lws_protocols* proto = lws_get_protocol(wsi);

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

}
}
}
