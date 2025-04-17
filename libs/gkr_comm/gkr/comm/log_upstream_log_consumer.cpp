#include <gkr/defs.hpp>

#include <gkr/comm/log_upstream_log_consumer.hpp>
#include <gkr/comm/constants.hpp>
#include <gkr/comm/registry.hpp>

#include <gkr/comm/log.hxx>

#include <gkr/params.hpp>
#include <gkr/log/logging.hpp>
#include <gkr/log/c_consumer.hpp>

extern "C"
{
int gkr_comm_add_upstream_log_consumer(
    void* channel,
    void* param,
    const struct gkr_comm_upstream_log_consumer_callbacks* callbacks,
    const struct gkr_params* parameters,
    size_t root
    )
{
    Check_Arg_NotNull(parameters, -1);

    const gkr::params& params = *reinterpret_cast<const gkr::params*>(parameters);

    const struct gkr_log_consumer_opt_callbacks* opt_callbacks = (callbacks == nullptr) ? nullptr : &callbacks->opt_callbacks;

    std::shared_ptr<gkr::log::consumer> consumer(new gkr::log::c_consumer<gkr::comm::upstream_log_consumer>(param, opt_callbacks, params, root));

    return gkr_log_add_consumer(channel, consumer);
}

int gkr_comm_add_upstream_log_consumer_ex(
    void* channel,
    void* param,
    const struct gkr_comm_upstream_log_consumer_callbacks* callbacks,
    const char* url,
    const char* transport,
    const char* provider_name,
    const struct gkr_params* parameters,
    size_t root
    )
{
    Check_Arg_NotNull(url, -1);

    const gkr::params* params = reinterpret_cast<const gkr::params*>(parameters);

    const struct gkr_log_consumer_opt_callbacks* opt_callbacks = (callbacks == nullptr) ? nullptr : &callbacks->opt_callbacks;

    std::shared_ptr<gkr::log::consumer> consumer(new gkr::log::c_consumer<gkr::comm::upstream_log_consumer>(param, opt_callbacks, url, transport, provider_name, params, root));

    return gkr_log_add_consumer(channel, consumer);
}
}

namespace gkr
{
namespace comm
{

upstream_log_consumer::upstream_log_consumer(
    const params& parameters,
    std::size_t root
    )
{
    configure(parameters, root);
}

upstream_log_consumer::upstream_log_consumer(
    const char* url,
    const char* transport,
    const char* provider_name,
    const params* parameters,
    std::size_t root
    )
{
    configure(url, transport, provider_name, parameters, root);
}

upstream_log_consumer::~upstream_log_consumer()
{
    if(m_bridge)
    {
        m_bridge->leave();
        m_bridge.reset();
    }
}

void upstream_log_consumer::reset(const params* parameters, std::size_t root)
{
    m_url   .clear();
    m_bridge.reset();

    m_params  = parameters;
    m_root    = root;
    m_format  = 0;
    m_version = 0;
}

bool upstream_log_consumer::configure(
    const params& parameters,
    std::size_t root
    )
{
    Check_ValidState(!m_started, false);

    reset(&parameters, root);

    m_params = &parameters;
    m_root   = root;

    url_parts parts {};
    std::string transport, provider_name, format, path;

    {
        auto lock = m_params->get_reader_lock();

        provider_name = m_params->get_value(COMM_PARAM_PROTOCOL_SERVICE_PROVIDER, m_root, "");
        transport     = m_params->get_value(COMM_PARAM_PROTOCOL_TRANSPORT       , m_root, "");
        format        = m_params->get_value(COMM_PARAM_PROTOCOL_FORMAT          , m_root, *SUPPORTED_FORMATS);
        parts.host    = m_params->get_value(COMM_PARAM_PROTOCOL_REMOTE_SERVER   , m_root, "localhost");

        for(m_version = 0, m_format = 0; m_format < std::size(SUPPORTED_FORMATS); ++m_format)
        {
            if(!format.compare(SUPPORTED_FORMATS[m_format])) { m_version = FORMAT_MAX_VERSION[m_format]; break; }
        }
        m_version = m_params->get_value(COMM_PARAM_PROTOCOL_VERSION, m_root, m_version);

        if(m_format >= std::size(SUPPORTED_FORMATS)) return false;
        if(m_version > FORMAT_MAX_VERSION[m_format]) return false;

        if(transport.empty()) return false;

        if(!transport.compare(COMM_TRANSPORT_WEB_SOCKET_SECURE))
        {
            parts.scheme = "wss";
            parts.port   = m_params->get_value(COMM_PARAM_PROTOCOL_REMOTE_PORT, m_root, COMM_PORT_LOG_UPSTREAM_WEB_SOCKET_SECURE);
        }
        else if(!transport.compare(COMM_TRANSPORT_WEB_SOCKET_PLAIN))
        {
            parts.scheme = "ws";
            parts.port   = m_params->get_value(COMM_PARAM_PROTOCOL_REMOTE_PORT, m_root, COMM_PORT_LOG_UPSTREAM_WEB_SOCKET_PLAIN);
        }
        else
        {
            parts.scheme = nullptr;
            parts.port   = m_params->get_value(COMM_PARAM_PROTOCOL_REMOTE_PORT, m_root, 0);
        }
        path.append(1, '/');
        path.append(format);
        path.append(1, '/');
        path.append(1, 'v');
        path.append(std::to_string(m_version));
        parts.path = path.c_str();

        if(!m_url.construct(parts)) return false;
    }

    provider* service_provider = registry::find_provider(provider_name.c_str());
    if(service_provider == nullptr) return false;

    m_bridge = service_provider->create_bridge(this, COMM_SERVICE_NAME_LOG_UPSTREAM_CLIENT, transport.c_str());
    if(m_bridge == nullptr) return false;

    return is_successfully_configured();
}

bool upstream_log_consumer::configure(
    const char* url,
    const char* transport,
    const char* provider_name,
    const params* parameters,
    std::size_t root
    )
{
    Check_Arg_NotNull(url, );

    Check_ValidState(!m_started, false);

    reset(parameters, root);

    m_url.decompose(url);

    if(m_url.parts().host == nullptr) return false;
    if(m_url.parts().path == nullptr) return false;

    if(!parse_path(m_url.parts().path)) return false;

    if((transport == nullptr) && (m_url.parts().scheme != nullptr))
    {
        if(!std::strcmp(m_url.parts().scheme, "wss")) transport = COMM_TRANSPORT_WEB_SOCKET_SECURE;
        if(!std::strcmp(m_url.parts().scheme, "ws" )) transport = COMM_TRANSPORT_WEB_SOCKET_PLAIN ;
    }
    if(transport == nullptr) return false;

    url_parts parts = m_url.parts();

    if(!std::strcmp(transport, COMM_TRANSPORT_WEB_SOCKET_SECURE))
    {
        parts.scheme = "wss";
        if(parts.port == 0) parts.port = COMM_PORT_LOG_UPSTREAM_WEB_SOCKET_SECURE;
    }
    else if(!std::strcmp(transport, COMM_TRANSPORT_WEB_SOCKET_PLAIN))
    {
        parts.scheme = "wss";
        if(parts.port == 0) parts.port = COMM_PORT_LOG_UPSTREAM_WEB_SOCKET_SECURE;
    }
    else
    {
        parts.scheme = nullptr;
    }
    m_url.construct(parts);

    provider* service_provider = registry::find_provider(provider_name);
    if(service_provider == nullptr) return false;

    m_bridge = service_provider->create_bridge(this, COMM_SERVICE_NAME_LOG_UPSTREAM_CLIENT, transport);
    if(m_bridge == nullptr) return false;

    return is_successfully_configured();
}

bool upstream_log_consumer::init_logging()
{
    if(!m_bridge) return false;

    configure_bridge();

    m_bridge->connect();

    m_started = true;

    return true;
}

void upstream_log_consumer::done_logging()
{
    if(!m_bridge) return;

    m_bridge->close();

    m_started = false;
}

bool upstream_log_consumer::filter_log_message(const log::message& msg)
{
    return false;
}

void upstream_log_consumer::consume_log_message(const log::message& msg)
{
    if(m_conn == nullptr) return;

    std::size_t capacity;

    void* buff = m_bridge->acquire_outgoing_buffer(capacity, m_conn);

    if(buff == nullptr) return;

    // setting data

    m_bridge->release_outgoing_buffer(0);
}

const char* upstream_log_consumer::compose_output(const log::message& msg, unsigned* len, int flags)
{
    return nullptr;
}

void upstream_log_consumer::on_error(int event, const void* data, std::size_t)
{
}

void upstream_log_consumer::on_connect(connection* conn)
{
}

void upstream_log_consumer::on_disconnect(connection* conn)
{
}

void upstream_log_consumer::on_data_received(connection* conn)
{
}

bool upstream_log_consumer::parse_path(const char* path)
{
    m_format  = std::size(SUPPORTED_FORMATS);
    m_version = unsigned(-1);

    Check_NotNullPtr(path, false);

    const char* pos1 = std::strchr(m_url.parts().path, '/');
    if(pos1 == nullptr) return false;
    const char* pos2 = std::strchr(++pos1, '/');
    if(pos2 == nullptr) return false;

    const std::size_t len = std::size_t(pos2++ - pos1);

    for(m_format = 0; m_format < std::size(SUPPORTED_FORMATS); ++m_format)
    {
        if(!std::memcmp(pos1, SUPPORTED_FORMATS[m_format], len) && (SUPPORTED_FORMATS[m_format][len] == 0)) break;
    }
    if(*pos2++ != 'v') return false;
    if(!std::isdigit(*pos2)) return false;

    int number = 0;
    while(std::isdigit(*pos2))
    {
        number = (number * 10) + (*pos2++ - '0');
    }
    if(*pos2 != 0) return false;
    m_version = number;

    if(m_format == std::size(SUPPORTED_FORMATS)) return false;
    if(m_version > FORMAT_MAX_VERSION[m_format]) return false;

    return true;
}

void upstream_log_consumer::configure_bridge()
{
    std::size_t max_count  = 0;
    std::size_t init_count = gkr_log_get_max_queue_entries();
    std::size_t init_size  = gkr_log_get_max_message_chars();
    float       res_factor = 1.5f;

    if(m_params != nullptr)
    {
        auto lock = m_params->get_reader_lock();

        max_count  = m_params->get_value(COMM_PARAM_BRIDGE_SEND_QUEUE_MAX_ELEMENT_COUNT , m_root, max_count );
        init_count = m_params->get_value(COMM_PARAM_BRIDGE_SEND_QUEUE_INIT_ELEMENT_COUNT, m_root, init_count);
        init_size  = m_params->get_value(COMM_PARAM_BRIDGE_SEND_QUEUE_INIT_ELEMENT_SIZE , m_root, init_size );
        res_factor = m_params->get_value(COMM_PARAM_BRIDGE_SEND_QUEUE_RESERVE_FACTOR    , m_root, res_factor);
    }

    m_bridge->configure_outgoing_queue(max_count, init_count, init_size, res_factor);

    LOGV_("Log Upstream configured bridge send queue with %zu elements with %zu bytes each - with res factor: %.2f", init_count, init_size, res_factor);
}

}
}
