#include <gkr/defs.hpp>

#include <gkr/comm/lws_protocol.hpp>

#include <gkr/diagnostics.hpp>
#include <gkr/log/defs/generic_cdefs.hpp>

#include <libwebsockets.h>

namespace libWebSocket
{

struct per_vhost_data
{
    struct lws_context*         context;
    struct lws_vhost*           vhost;
    const struct lws_protocols* protocol;
};
struct per_session_data
{
    struct lws* wsi;
};

int main_protocol_callback(struct lws* wsi, enum lws_callback_reasons reason, void* user, void* in, std::size_t len)
{
    struct per_session_data* psd = (struct per_session_data*)user;

//  struct per_vhost_data* vhd = (struct per_vhost_data*)lws_protocol_vh_priv_get(lws_get_vhost(wsi), lws_get_protocol(wsi));

    const struct lws_protocols* protocol = lws_get_protocol(wsi);

    switch(reason)
    {
        case LWS_CALLBACK_PROTOCOL_INIT:
            //vhd = (struct per_vhost_data*)lws_protocol_vh_priv_zalloc(lws_get_vhost(wsi), lws_get_protocol(wsi), sizeof(struct per_vhost_data));
            //vhd->context  = lws_get_context(wsi);
            //vhd->vhost    = lws_get_vhost(wsi);
            //vhd->protocol = lws_get_protocol(wsi);
            LOGI_("Protocol Init: %s", protocol->name);
            static_cast<Protocol*>(protocol->user)->on_init();
            break;

        case LWS_CALLBACK_PROTOCOL_DESTROY:
            static_cast<Protocol*>(protocol->user)->on_done();
            LOGI_("Protocol Destroy: %s", protocol->name);
            break;

        case LWS_CALLBACK_ESTABLISHED:
            LOGI_("Connection opened: %s", protocol->name);
            static_cast<Protocol*>(protocol->user)->on_connection_opened();
            break;

        case LWS_CALLBACK_CLOSED:
            static_cast<Protocol*>(protocol->user)->on_connection_closed();
            LOGI_("Connection closed: %s", protocol->name);
            break;

        case LWS_CALLBACK_RECEIVE:
            LOGI_("Connection data received: %s", protocol->name);
            static_cast<Protocol*>(protocol->user)->on_connection_received_data();
            break;

        case LWS_CALLBACK_CLIENT_WRITEABLE:
            LOGI_("Connection client received: %s", protocol->name);
            static_cast<Protocol*>(protocol->user)->on_connection_client_writeable();
            break;

        case LWS_CALLBACK_SERVER_WRITEABLE:
            LOGI_("Connection server received: %s", protocol->name);
            static_cast<Protocol*>(protocol->user)->on_connection_server_writeable();
            break;

        default:
            LOGW_("Unknown lws callback reason: %i", reason);
            break;
    }
    return 0;
}

Protocol::Protocol()
{
}

Protocol::~Protocol()
{
}

}
