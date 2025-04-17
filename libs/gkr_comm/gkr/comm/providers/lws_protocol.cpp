#include <gkr/defs.hpp>

#include <gkr/comm/providers/lws_protocol.hpp>

#include <gkr/comm/providers/lws_instance.hpp>

#include <gkr/comm/log.hxx>

#include <gkr/comm/bridge.hpp>
#include <gkr/misc/sentry.hpp>

#include <libwebsockets.h>

namespace gkr
{
namespace comm
{
namespace providers
{
namespace libwebsocket
{

protocol::~protocol() noexcept
{
    if(m_bridge != nullptr)
    {
        m_bridge->on_event(bridge::event_leave);
        m_bridge.reset();
    }
}

bool protocol::connect()
{
    Check_ValidState(is_client(), false);
    Check_ValidState((m_port == CONTEXT_PORT_NO_LISTEN), false);

    return true;
}

bool protocol::listen()
{
    Check_ValidState(is_server(), false);
    Check_ValidState((m_port != CONTEXT_PORT_NO_LISTEN), false);

    return true;
}

void protocol::on_data_sent(connection* conn)
{
    instance* inst = static_cast<instance*>(conn);

    inst->request_write();
}

void protocol::close(connection* conn, int status, const char* reason)
{
    instance* inst = static_cast<instance*>(conn);
}

void protocol::on_init()
{
    m_bridge->on_event(bridge::event_init);
}

void protocol::on_done()
{
    m_bridge->on_event(bridge::event_done);
}

void protocol::on_connection_opened(instance* inst)
{
    m_bridge->on_event(bridge::event_connect, inst);
}

void protocol::on_connection_closed(instance* inst)
{
    m_bridge->on_event(bridge::event_disconnect, inst);
}

void protocol::on_connection_pong(instance* inst)
{
    m_bridge->on_event(bridge::event_pong, inst);
}

void protocol::on_connection_writeable(instance* inst)
{
    if(!can_send_data())
    {
        Check_ValidState(!m_bridge->have_sent_data(inst), );
        return;
    }
    std::size_t size;
    const void* data = m_bridge->acquire_sent_data(size, inst);
    if(data == nullptr) return;

    misc::sentry sentry([this] () {
        m_bridge->release_sent_data();
    });
    inst->write_data(data, size);
}

void protocol::on_connection_received_data(instance* inst, const void* data, std::size_t size)
{
    std::size_t capacity;

    void* buff = m_bridge->acquire_incoming_buffer(capacity, inst);
    if(buff == nullptr) return;

    misc::sentry sentry([this, size] () {
        m_bridge->release_incoming_buffer(size);
    });
    if(size > capacity)
    {
        buff = m_bridge->reserve_incoming_buffer(size);
        if(buff == nullptr) return;
    }
    std::memcpy(buff, data, size);
}

//struct per_vhost_data
//{
//    const
//    struct lws_protocols* proto;
//    struct lws_context*   context;
//    struct lws_vhost*     vhost;
//};
//struct per_vhost_data* vhd = (struct per_vhost_data*)lws_protocol_vh_priv_get(lws_get_vhost(wsi), lws_get_protocol(wsi));
//vhd = (struct per_vhost_data*)lws_protocol_vh_priv_zalloc(lws_get_vhost(wsi), lws_get_protocol(wsi), sizeof(struct per_vhost_data));
//vhd->context  = lws_get_context(wsi);
//vhd->vhost    = lws_get_vhost(wsi);
//vhd->protocol = lws_get_protocol(wsi);

int dummy_callback(struct lws* wsi, enum lws_callback_reasons reason, void* user, void* data, size_t size)
{
    const struct lws_protocols* proto = lws_get_protocol(wsi);

    if((proto != nullptr) && (proto->user != nullptr))
    {
        static_cast<dummy_protocol*>(proto->user)->callback(reason, data, size);
    }
    return lws_callback_http_dummy(wsi, reason, user, data, size);
}
dummy_protocol::~dummy_protocol() noexcept
{
}
void* dummy_protocol::get_callback() noexcept
{
    return dummy_callback;
}
unsigned dummy_protocol::get_info(std::size_t& rx_size, std::size_t& tx_size)
{
    rx_size = 0;
    tx_size = 0;
    return 0;
}
void dummy_protocol::callback(int reason, const void* data, std::size_t size)
{
}

int server_callback(struct lws* wsi, enum lws_callback_reasons reason, void* user, void* in, std::size_t len)
{
    instance* inst = reinterpret_cast<instance*>(user);

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
            new (inst) instance(wsi);
            static_cast<server_protocol*>(proto->user)->on_connection_opened(inst);
            break;

        case LWS_CALLBACK_CLOSED:
            static_cast<server_protocol*>(proto->user)->on_connection_closed(inst);
            inst->~instance();
            break;

        case LWS_CALLBACK_SERVER_WRITEABLE:
            static_cast<server_protocol*>(proto->user)->on_connection_writeable(inst);
            break;

        case LWS_CALLBACK_RECEIVE:
            static_cast<server_protocol*>(proto->user)->on_connection_received_data(inst, in, len);
            break;

        case LWS_CALLBACK_RECEIVE_PONG:
            static_cast<server_protocol*>(proto->user)->on_connection_pong(inst);
            break;

        default:
            LOGV_("libwebsocket other server reason: %i", reason);
            break;
    }
    return 0;
}

server_protocol::~server_protocol() noexcept
{
}
void* server_protocol::get_callback() noexcept
{
    return server_callback;
}
unsigned server_protocol::get_info(std::size_t& rx_size, std::size_t& tx_size)
{
    rx_size = 512;
    tx_size = 512;
    return 0;
}

int client_callback(struct lws* wsi, enum lws_callback_reasons reason, void* user, void* in, std::size_t len)
{
    instance* inst = reinterpret_cast<instance*>(user);

    const struct lws_protocols* proto = lws_get_protocol(wsi);

    if(proto == nullptr)
    {
    }
    else switch(reason)
    {
        case LWS_CALLBACK_PROTOCOL_INIT:
            static_cast<client_protocol*>(proto->user)->on_init();
            break;

        case LWS_CALLBACK_PROTOCOL_DESTROY:
            static_cast<client_protocol*>(proto->user)->on_done();
            break;

        case LWS_CALLBACK_CLIENT_CONNECTION_ERROR:
            static_cast<client_protocol*>(proto->user)->on_connect_error(static_cast<char*>(in), len);
            break;

        case LWS_CALLBACK_CLIENT_ESTABLISHED:
            new (inst) instance(wsi);
            static_cast<client_protocol*>(proto->user)->on_connection_opened(inst);
            break;

        case LWS_CALLBACK_CLIENT_CLOSED:
            static_cast<client_protocol*>(proto->user)->on_connection_closed(inst);
            inst->~instance();
            break;

        case LWS_CALLBACK_CLIENT_WRITEABLE:
            static_cast<client_protocol*>(proto->user)->on_connection_writeable(inst);
            break;

        case LWS_CALLBACK_CLIENT_RECEIVE:
            static_cast<client_protocol*>(proto->user)->on_connection_received_data(inst, in, len);
            break;

        case LWS_CALLBACK_CLIENT_RECEIVE_PONG:
            static_cast<client_protocol*>(proto->user)->on_connection_pong(inst);
            break;

        default:
            LOGV_("libwebsocket other client reason: %i", reason);
            break;
    }
    return 0;
}

client_protocol::~client_protocol() noexcept
{
}
void* client_protocol::get_callback() noexcept
{
    return client_callback;
}
unsigned client_protocol::get_info(std::size_t& rx_size, std::size_t& tx_size)
{
    rx_size = 512;
    tx_size = 512;
    return 0;
}

void client_protocol::on_connect_error(char* reason, std::size_t len)
{
    m_bridge->on_event(bridge::event_failed_connect, reason, len);
}

#if 0
bool client_protocol::connect(const char* url, const char* protocol)
{
//  Check_Arg_NotNull(url, false);
//
//  if(m_connect_url.is_valid()) return false;
//
//  m_connect_url.decompose(url);
//
//  if(!m_connect_url.is_valid()) return false;
//
//  struct lws_client_connect_info info;
//  std::memset(&info, 0, sizeof(info));
//
//  int ssl_connection;
//
//  if     (!std::strcmp(m_connect_url.parts().scheme, "wss")) {ssl_connection = LCCSCF_USE_SSL; }
//  else if(!std::strcmp(m_connect_url.parts().scheme, "ws" )) {ssl_connection = 0; }
//  else
//  {
//      m_connect_url.clear();
//      return false;
//  }
//  info.context               = get_parent_context();
//  info.port                  = m_connect_url.parts().port;
//  info.address               = m_connect_url.parts().host;
//  info.path                  = m_connect_url.parts().path;
//  info.host                  = info.address;
//  info.origin                = info.address;
//  info.ssl_connection        = ssl_connection;
//  info.protocol              = protocol;
//  info.local_protocol_name   = get_name();
//  info.retry_and_idle_policy = &retry;

    return true;
}
#endif
}
}
}
}
