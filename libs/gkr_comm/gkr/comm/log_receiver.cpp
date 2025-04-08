#include <gkr/defs.hpp>

#include <gkr/comm/log_receiver.hpp>

#include <gkr/comm/constants.hpp>
#include <gkr/comm/registry.hpp>
#include <gkr/comm/log_collector.hpp>

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
    Check_Arg_NotNull(transport, false);

    Check_Arg_IsValid((port > 0) && (port < 65536), false);

    m_params = parameters;
    m_root   = root;
    m_port   = port;

    provider* service_provider = registry::find_provider(provider_name);
    if(service_provider == nullptr) return false;

    m_bridge = service_provider->create_bridge(COMM_SERVICE_NAME_LOG_UPSTREAM_SERVER, transport, this);
    if(!m_bridge) return false;

    return true;
}

bool log_receiver::configure(
    const params& parameters,
    std::size_t root
    )
{
    Check_ValidState(!running(), false);

    m_params = &parameters;
    m_root   = root;

    std::string transport, provider_name;

    {
        auto lock = m_params->get_reader_lock();

        provider_name = m_params->get_value(COMM_PARAM_PROTOCOL_SERVICE_PROVIDER, m_root, "");
        transport     = m_params->get_value(COMM_PARAM_PROTOCOL_TRANSPORT       , m_root, "");

        if(transport.empty()) return false;

        if(!transport.compare(COMM_TRANSPORT_WEB_SOCKET_SECURE))
        {
            m_port = m_params->get_value(COMM_PARAM_PROTOCOL_LISTEN_PORT, m_root, COMM_PORT_LOG_UPSTREAM_WEB_SOCKET_SECURE);
        }
        else if(!transport.compare(COMM_TRANSPORT_WEB_SOCKET_PLAIN))
        {
            m_port = m_params->get_value(COMM_PARAM_PROTOCOL_LISTEN_PORT, m_root, COMM_PORT_LOG_UPSTREAM_WEB_SOCKET_PLAIN);
        }
        else
        {
            m_port = m_params->get_value(COMM_PARAM_PROTOCOL_LISTEN_PORT, m_root, 0);
        }
    }
    if((m_port <= 0) || (m_port >= 65536)) return false;

    provider* service_provider = registry::find_provider(provider_name.c_str());
    if(service_provider == nullptr) return false;

    m_bridge = service_provider->create_bridge(COMM_SERVICE_NAME_LOG_UPSTREAM_SERVER, transport.c_str(), this);
    if(!m_bridge) return false;

    return true;
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

    return true;
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

}
}
