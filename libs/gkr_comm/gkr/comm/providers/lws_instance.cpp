#include <gkr/defs.hpp>

#include <gkr/comm/providers/lws_instance.hpp>

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

instance::instance(struct lws* wsi) : m_wsi(wsi)
{
    Assert_NotNullPtr(wsi);

    char buf[256];

    m_addr.assign(lws_get_peer_simple(wsi, buf, sizeof(buf)));

    const int len = lws_hdr_copy(wsi, buf, sizeof(buf), WSI_TOKEN_GET_URI);

    m_path.assign(buf, len);
}

instance::~instance()
{
}

const char* instance::addr() const
{
    return m_addr.c_str();
}

const char* instance::path() const
{
    return m_path.c_str();
}

void instance::request_write()
{
    lws_callback_on_writable(m_wsi);
}

bool instance::write_data(const void* data, std::size_t size)
{
    return false;
}

}
}
}
}
