#include <gkr/defs.hpp>

#include <gkr/comm/web_socket_log_consumer.hpp>
#include <gkr/comm/constants.hpp>
#include <gkr/comm/registry.hpp>
#include <gkr/params.hpp>

#include <gkr/log/logging.hpp>
#include <gkr/log/c_consumer.hpp>

extern "C"
{
int gkr_comm_add_web_socket_log_consumer(
    void* channel,
    void* param,
    const struct gkr_comm_web_socket_log_consumer_callbacks* callbacks,
    struct gkr_params* parameters,
    size_t root
    )
{
    Check_Arg_NotNull(parameters);

    gkr::params& params = *reinterpret_cast<gkr::params*>(parameters);

    const struct gkr_log_consumer_opt_callbacks* opt_callbacks = (callbacks == nullptr) ? nullptr : &callbacks->opt_callbacks;

    std::shared_ptr<gkr::log::consumer> consumer(new gkr::log::c_consumer<gkr::comm::web_socket_log_consumer>(param, opt_callbacks, params, root));

    return gkr_log_add_consumer(channel, consumer);
}

int gkr_comm_add_web_socket_log_consumer_ex(
    void* channel,
    void* param,
    const struct gkr_comm_web_socket_log_consumer_callbacks* callbacks,
    const char* url,
    const char* provider_name,
    struct gkr_params* parameters,
    size_t root
    )
{
    Check_Arg_NotNull(url);

    gkr::params* params = reinterpret_cast<gkr::params*>(parameters);

    const struct gkr_log_consumer_opt_callbacks* opt_callbacks = (callbacks == nullptr) ? nullptr : &callbacks->opt_callbacks;

    std::shared_ptr<gkr::log::consumer> consumer(new gkr::log::c_consumer<gkr::comm::web_socket_log_consumer>(param, opt_callbacks, url, provider_name, params, root));

    return gkr_log_add_consumer(channel, consumer);
}
}

namespace gkr
{
namespace comm
{

web_socket_log_consumer::web_socket_log_consumer(
    params& parameters,
    std::size_t root
    )
    : m_params(&parameters)
    , m_root  (root)
{
    const char* provider_name = m_params->get_value(COMM_PARAM_WEB_SOCKET_PROVIDER, m_root, nullptr);

    provider* p = registry::find_provider(provider_name);

    Check_ValidState(p, );

    m_bridge = p->create_bridge(COMM_SERVICE_NAME_LOG_CONSUMER, this);

    Check_ValidState(m_bridge, );

    auto& parts = m_url.modify_parts();
    parts.scheme = m_params->get_value(COMM_PARAM_URL_SCHEME, m_root, "wss");
    parts.host   = m_params->get_value(COMM_PARAM_URL_HOST  , m_root, "localhost");
    parts.port   = m_params->get_value(COMM_PARAM_URL_PORT  , m_root, COMM_PORT_WEB_SOCKET_LOG_UPSTREAM);
    parts.path   = m_params->get_value(COMM_PARAM_URL_PATH  , m_root, DEFAULT_PATH);

    //gkr_url_validate
}
web_socket_log_consumer::web_socket_log_consumer(
    const char* url,
    const char* provider_name,
    params* parameters,
    std::size_t root
    )
    : m_params(parameters)
    , m_root  (root)
{
    Check_Arg_NotNull(url, );

    provider* p = registry::find_provider(provider_name);

    Check_ValidState(p, );

    m_bridge = p->create_bridge(COMM_SERVICE_NAME_LOG_CONSUMER, this);

    Check_ValidState(m_bridge, );

    m_url.reset(url);

    //gkr_url_validate
}

web_socket_log_consumer::~web_socket_log_consumer()
{
    m_bridge.reset();
}

bool web_socket_log_consumer::init_logging()
{
    if(!m_bridge) return false;

    m_bridge->connect();

    return true;
}

void web_socket_log_consumer::done_logging()
{
    if(!m_bridge) return;

    m_bridge->close();
}

bool web_socket_log_consumer::filter_log_message(const log::message& msg)
{
    return false;
}

void web_socket_log_consumer::consume_log_message(const log::message& msg)
{
    if(!m_bridge->is_connected()) return;

    std::size_t capacity;

    void* buff = m_bridge->acquire_outgoing_buffer(capacity);

    if(buff == nullptr) return;

    // setting data

    m_bridge->release_outgoing_buffer(0);
}

const char* web_socket_log_consumer::compose_output(const log::message& msg, unsigned* len, int flags)
{
    return nullptr;
}

bool web_socket_log_consumer::on_error(int evt, void* data, std::size_t)
{
    return false;
}

void web_socket_log_consumer::on_connect()
{
}

void web_socket_log_consumer::on_disconnect()
{
}

void web_socket_log_consumer::on_data_received()
{
}

}
}
