#include <gkr/defs.hpp>

#include <gkr/comm/log_receiver.hpp>

#include <gkr/comm/constants.hpp>
#include <gkr/comm/registry.hpp>
#include <gkr/comm/log_collector.hpp>

#include <gkr/comm/log.hxx>

#include <gkr/params.hpp>

namespace gkr
{
namespace comm
{

log_collector::~log_collector()
{
}

log_receiver::log_receiver()
{
    check_args_order();
}

log_receiver::~log_receiver() noexcept(DIAG_NOEXCEPT)
{
    join(true);
}

bool log_receiver::configure(
    int port,
    const char* transport,
    const char* provider_name,
    const params* parameters,
    std::size_t root
    )
{
    Check_ValidState(!running(), false);

    if((port <= 0) || (port >= 65536))
    {
        LOGE_("Log Receiver configuration failed - invalid port: %i", port);
        return false;
    }
    if((transport == nullptr) || (*transport == 0))
    {
        LOGE("Log Receiver configuration failed - transport not specified");
        return false;
    }
    provider* service_provider = registry::find_provider(provider_name);
    if(service_provider == nullptr)
    {
        if(provider_name == nullptr) provider_name = "<NULL>";
        LOGE_("Log Receiver configuration failed - cannot find provider with name: (%s)", provider_name);
        return false;
    }
    auto bridge = service_provider->create_bridge(this, COMM_SERVICE_NAME_LOG_UPSTREAM_SERVER, transport, port);
    if(bridge == nullptr) return false;

    m_bridge.reset();

    m_params = parameters;
    m_root   = root;
    m_bridge = bridge;

    LOGI_("Log Receiver configured on port %i with transport '%s' and provider '%s'", port, transport, service_provider->get_name());
    return true;
}

bool log_receiver::configure(
    const params& parameters,
    std::size_t root
    )
{
    Check_ValidState(!running(), false);

    int port;
    std::string transport, provider_name;
    {
        auto lock = parameters.get_reader_lock();

        provider_name = parameters.get_value(COMM_PARAM_PROTOCOL_SERVICE_PROVIDER, root, "");
        transport     = parameters.get_value(COMM_PARAM_PROTOCOL_TRANSPORT       , root, "");

        if(transport.empty())
        {
            port = 0;
        }
        else if(!transport.compare(COMM_TRANSPORT_WEB_SOCKET_SECURE))
        {
            port = parameters.get_value(COMM_PARAM_PROTOCOL_LISTEN_PORT, root, COMM_PORT_LOG_UPSTREAM_WEB_SOCKET_SECURE);
        }
        else if(!transport.compare(COMM_TRANSPORT_WEB_SOCKET_PLAIN))
        {
            port = parameters.get_value(COMM_PARAM_PROTOCOL_LISTEN_PORT, root, COMM_PORT_LOG_UPSTREAM_WEB_SOCKET_PLAIN);
        }
        else
        {
            port = parameters.get_value(COMM_PARAM_PROTOCOL_LISTEN_PORT, root, 0);
        }
    }
    return configure(port, transport.c_str(), provider_name.c_str(), &parameters, root);
}

const char* log_receiver::get_name() noexcept
{
    return "log-receiver";
}

long long log_receiver::get_wait_timeout_ns() noexcept
{
    return WAIT_INFINITE;
}

std::size_t log_receiver::get_waitable_objects_count() noexcept
{
    return 1;
}

waitable_object& log_receiver::get_waitable_object(std::size_t index) noexcept
{
    Check_ValidState(index == 0);
    return m_data_received_event;
}

bool log_receiver::on_start()
{
    if(!m_bridge) return false;

    configure_bridge();

    return m_bridge->listen();
}

void log_receiver::on_finish()
{
}

void log_receiver::on_cross_action(action_id_t action, void* param, void* result)
{
}

void log_receiver::on_queue_action(action_id_t action, void* data)
{
}

void log_receiver::on_wait_timeout()
{
    Check_Failure();
}

void log_receiver::on_wait_success(std::size_t index)
{
}

bool log_receiver::on_exception(except_method_t method, const std::exception* e) noexcept
{
    return true;
}

bool log_receiver::on_error(int evt, void* data, std::size_t size)
{
    return false;
}

void log_receiver::on_connect()
{
}

void log_receiver::on_disconnect()
{
}

void log_receiver::on_data_received()
{
}

bool log_receiver::add_collector(std::shared_ptr<log_collector> collector)
{
    return false;
}

bool log_receiver::del_collector(std::shared_ptr<log_collector> collector)
{
    return false;
}

void log_receiver::configure_bridge()
{
    std::size_t init_count = 64;
    std::size_t init_size  = 1024;
    float       res_factor = 1.5f;

    if(m_params != nullptr)
    {
        auto lock = m_params->get_reader_lock();

        init_count = m_params->get_value(COMM_PARAM_BRIDGE_RECV_QUEUE_INIT_ELEMENT_COUNT, m_root, init_count);
        init_size  = m_params->get_value(COMM_PARAM_BRIDGE_RECV_QUEUE_INIT_ELEMENT_SIZE , m_root, init_size );
        res_factor = m_params->get_value(COMM_PARAM_BRIDGE_RECV_QUEUE_RESERVE_FACTOR    , m_root, res_factor);
    }

    m_bridge->configure_incoming_queue(init_count, init_size, res_factor);

    LOGV_("Log Receiver configured bridge receive queue with %zu elements with %zu bytes each - with res factor: %.2f", init_count, init_size, res_factor);
}

}
}
