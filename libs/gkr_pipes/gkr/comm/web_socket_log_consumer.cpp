#include <gkr/defs.hpp>

#include <gkr/comm/web_socket_log_consumer.hpp>
#include <gkr/comm/names.hpp>
#include <gkr/comm/registry.hpp>

#include <gkr/log/logging.hpp>

namespace gkr
{
namespace comm
{

web_socket_log_consumer::web_socket_log_consumer(
    const char* url,
    const char* provider_name
    )
{
    provider* p = registry::find_provider(provider_name);

    if(p != nullptr)
    {
        m_bridge = p->create_bridge(GKR_COMM_SERVICE_LOG_CONSUMER, this);
    }
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
