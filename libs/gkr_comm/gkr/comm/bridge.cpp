#include <gkr/defs.hpp>

#include <gkr/comm/bridge.hpp>

#include <gkr/comm/service.hpp>
#include <gkr/comm/end_point.hpp>
#include <gkr/comm/connection.hpp>

#include <gkr/comm/log.hxx>

namespace gkr
{
namespace comm
{

connection::connection()
{
    static unsigned id = 0;
    m_id = ++id;
}
connection::~connection()
{
}

end_point::~end_point()
{
}
service::~service()
{
}

bridge::bridge(end_point* ep, service* s)
    : m_end_point(ep)
    , m_service  (s )
{
    Check_Arg_IsValid(ep, );
    Check_Arg_IsValid(s , );
}

bridge::~bridge()
{
}

bool bridge::configure_outgoing_queue(std::size_t max_count, std::size_t init_count, std::size_t init_size, float reserve_factor)
{
    Check_NotNullPtr(m_service, false);
    Check_ValidState(m_service->can_send_data(), false);

    Check_ValidState(m_connections.empty(), false);

    if(reserve_factor < MINIMUM_RESERVE_FACTOR) reserve_factor = MINIMUM_RESERVE_FACTOR;
    if(reserve_factor > MAXIMUM_RESERVE_FACTOR) reserve_factor = MAXIMUM_RESERVE_FACTOR;

    if(max_count < init_count) max_count = std::size_t(init_count * reserve_factor);

    m_outgoing_factor = reserve_factor;
    m_outgoing_size   = init_size + QUEUE_ENTRY_HEAD_SIZE;
    m_outgoing_count  = init_count;
    m_outgoing_maxcap = max_count;
    return true;
}

bool bridge::configure_incoming_queue(std::size_t max_count, std::size_t init_count, std::size_t init_size, float reserve_factor)
{
    Check_NotNullPtr(m_service, false);
    Check_ValidState(m_service->can_receive_data(), false);

    Check_ValidState(m_incoming_queue.empty(), false);

    if(reserve_factor < MINIMUM_RESERVE_FACTOR) reserve_factor = MINIMUM_RESERVE_FACTOR;
    if(reserve_factor > MAXIMUM_RESERVE_FACTOR) reserve_factor = MAXIMUM_RESERVE_FACTOR;
 
    if(max_count < init_count) max_count = std::size_t(init_count * reserve_factor);

    m_incoming_factor = reserve_factor;
    m_incoming_maxcap = max_count;

    init_size += QUEUE_ENTRY_HEAD_SIZE;

    m_incoming_queue.reset(init_count, init_size + QUEUE_ENTRY_HEAD_SIZE);
    return true;
}

bool bridge::connect()
{
    Check_NotNullPtr(m_service, false);
    Check_ValidState(m_service->is_client(), false);

    m_incoming_queue.threading.set_current_thread_as_exclusive_consumer();

    return m_service->connect();
}

bool bridge::listen()
{
    Check_NotNullPtr(m_service, false);
    Check_ValidState(m_service->is_server(), false);

    m_incoming_queue.threading.set_current_thread_as_exclusive_consumer();

    return m_service->listen();
}

void bridge::close(connection* conn, int status, const char* reason)
{
    if(m_service == nullptr)
    {
    }
    else if(conn != nullptr)
    {
        m_service->close(conn, status, reason);
    }
    else for(slot_t& slot : m_connections)
    {
        m_service->close(slot.conn, status, reason);
    }
}

void bridge::leave()
{
    m_end_point = nullptr;
}

void* bridge::acquire_outgoing_buffer(std::size_t& capacity, connection* conn)
{
    capacity = 0;

    Check_Arg_NotNull(conn, nullptr);

    Check_NotNullPtr(m_service, nullptr);
    Check_ValidState(m_service->can_send_data(), nullptr);

    Check_ValidState(m_outgoing_procuder_header == nullptr, nullptr);

    auto it = m_connections.find(conn);

    Check_ValidState(it != m_connections.end(), nullptr);

    queue_t& queue = it->queue;

    m_outgoing_procuder_header = queue.try_acquire_producer_element_ownership<header_t>();

    if(m_outgoing_procuder_header == nullptr)
    {
        std::size_t max_capacity = m_outgoing_maxcap;
        std::size_t old_capacity = queue.capacity();
        std::size_t new_capacity = std::size_t(float(old_capacity) * m_outgoing_factor);

        if(new_capacity <= old_capacity) new_capacity = (old_capacity + 1);
        if(new_capacity >  max_capacity) new_capacity = max_capacity;

        if(queue.reserve(new_capacity))
        {
            LOGV_("Comm bridge increased outgoing queue capacity from %zu to %zu for connection id %u", old_capacity, new_capacity, conn->id());
        }
        else
        {
            LOGW_("Comm bridge failed to increase outgoing queue capacity to %zu for connection id %u", new_capacity, conn->id());

            Check_NotNullPtr(m_end_point, nullptr);
            m_end_point->on_error(error_outgoing_queue_maxed, nullptr, 0);
        }
        m_outgoing_procuder_header = queue.try_acquire_producer_element_ownership<header_t>();

        if(m_outgoing_procuder_header == nullptr)
        {
            LOGW_("Comm bridge dropped data packet because outgoing queue is full for connection id %u", conn->id());
            return nullptr;
        }
    }
    m_outgoing_procuder_header->conn = conn;
    m_outgoing_procuder_header->size = 0;

    capacity = (queue.element_size() - QUEUE_ENTRY_HEAD_SIZE);

    return m_outgoing_procuder_header->data;
}

void* bridge::reserve_outgoing_buffer(std::size_t capacity)
{
    capacity += QUEUE_ENTRY_HEAD_SIZE;

    Check_NotNullPtr(m_outgoing_procuder_header, nullptr);

    connection* conn = m_outgoing_procuder_header->conn;

    auto it = m_connections.find(conn);

    Check_ValidState(it != m_connections.end(), nullptr);

    queue_t& queue = it->queue;

    const size_t new_stride = std::exchange(capacity, queue.element_size());

    if(queue.change_element_size(new_stride, reinterpret_cast<void**>(&m_outgoing_procuder_header)))
    {
        LOGV_("Comm bridge increased outgoing queue stride from %zu to %zu for connection id %u", capacity, new_stride, conn->id());
        return m_outgoing_procuder_header->data;
    }
    else
    {
        LOGW_("Comm bridge failed to increase outgoing queue stride to %zu for connection id %u", new_stride, conn->id());
        return nullptr;
    }
}

bool bridge::release_outgoing_buffer(std::size_t size)
{
    Check_NotNullPtr(m_outgoing_procuder_header, false);

    connection* conn = m_outgoing_procuder_header->conn;

    auto it = m_connections.find(conn);

    Check_ValidState(it != m_connections.end(), nullptr);

    queue_t& queue = it->queue;

    if(size == 0)
    {
        queue.cancel_producer_element_ownership(m_outgoing_procuder_header);
        return true;
    }
    if((size + QUEUE_ENTRY_HEAD_SIZE) > queue.element_size())
    {
        queue.cancel_producer_element_ownership(m_outgoing_procuder_header);
        Check_Arg_Invalid(size, false);
        return false;
    }
    m_outgoing_procuder_header->size = size;

    queue.release_producer_element_ownership(m_outgoing_procuder_header);

    Check_NotNullPtr(m_service, false);
    m_service->on_data_sent(conn);
    return true;
}

bool bridge::have_received_data()
{
    Check_NotNullPtr(m_service, false);
    Check_ValidState(m_service->can_receive_data(), false);

    return !m_incoming_queue.empty();
}

void* bridge::acquire_received_data(std::size_t& size, connection*& conn)
{
    size = 0;
    conn = nullptr;

    Check_ValidState(m_incoming_consumer_header == nullptr, nullptr);

    Check_NotNullPtr(m_service, nullptr);
    Check_ValidState(m_service->can_receive_data(), nullptr);

    m_incoming_consumer_header = m_incoming_queue.try_acquire_consumer_element_ownership<header_t>();

    if(m_incoming_consumer_header == nullptr) return nullptr;

    conn = m_incoming_consumer_header->conn;
    size = m_incoming_consumer_header->size;
    return m_incoming_consumer_header->data;
}

bool bridge::release_received_data(bool cancel)
{
    Check_ValidState(m_incoming_consumer_header != nullptr, false);

    if(cancel)
    {
        return m_incoming_queue.cancel_consumer_element_ownership(m_incoming_consumer_header);
    }
    else
    {
        return m_incoming_queue.release_consumer_element_ownership(m_incoming_consumer_header);
    }
}

void bridge::on_event(int event, void* data, std::size_t size)
{
    switch(event)
    {
        case event_init:
            m_incoming_queue.threading.set_current_thread_as_exclusive_producer();
            break;
        case event_done:
            m_connections.clear();
            break;
        case event_pong:
            break;
        case event_leave:
            m_service = nullptr;
            break;

        case event_connect:
            if(data != nullptr)
            {
                connection* conn = static_cast<connection*>(data);

                auto it = m_connections.find(nullptr);

                if(it == m_connections.end()) it = m_connections.insert();

                slot_t& slot = *it;

                slot.conn = conn;
                slot.queue.reset(m_outgoing_count, m_outgoing_size);

                slot.queue.threading = m_incoming_queue.threading.switched();

                if(m_end_point != nullptr) m_end_point->on_connect(conn);
            }
            break;
        case event_disconnect:
            if(data != nullptr)
            {
                connection* conn = static_cast<connection*>(data);

                if(m_end_point != nullptr) m_end_point->on_disconnect(conn);

                auto it = m_connections.find(conn);

                Assert_Check(it != m_connections.end());

                slot_t& slot = *it;
                slot.queue.clean();
                slot.conn = nullptr;
            }
            break;

        case event_failed_connect:
            if(m_end_point != nullptr) m_end_point->on_error(error_connection_failed, data, size);
            break;

        default:
            LOGW_("Comm bridge fired with unknown event %i", event);
            break;
    }
}

void* bridge::acquire_incoming_buffer(std::size_t& capacity, connection* conn)
{
    capacity = 0;

    Check_Arg_NotNull(conn, nullptr);

    Check_NotNullPtr(m_service, nullptr);
    if(!m_service->can_receive_data())
    {
        Check_NotNullPtr(m_end_point, nullptr);
        m_end_point->on_error(error_received_unexpected_data, conn, 0);
        return nullptr;
    }
    Check_ValidState(m_incoming_procuder_header == nullptr, nullptr);

    m_incoming_procuder_header = m_incoming_queue.try_acquire_producer_element_ownership<header_t>();

    if(m_incoming_procuder_header == nullptr)
    {
        std::size_t max_capacity = m_incoming_maxcap;
        std::size_t old_capacity = m_incoming_queue.capacity();
        std::size_t new_capacity = std::size_t(float(old_capacity) * m_incoming_factor);

        if(new_capacity <= old_capacity) new_capacity = (old_capacity + 1);
        if(new_capacity >  max_capacity) new_capacity = max_capacity;

        if(m_incoming_queue.reserve(new_capacity))
        {
            LOGV_("Comm bridge increased incoming queue capacity from %zu to %zu for connection id %u", old_capacity, new_capacity, conn->id());
        }
        else
        {
            LOGW_("Comm bridge failed to increase incoming queue capacity to %zu for connection id %u", new_capacity, conn->id());

            Check_NotNullPtr(m_end_point, nullptr);
            m_end_point->on_error(error_incoming_queue_maxed, nullptr, 0);
        }
        m_incoming_procuder_header = m_incoming_queue.try_acquire_producer_element_ownership<header_t>();

        if(m_incoming_procuder_header == nullptr)
        {
            LOGW_("Comm bridge dropped data packet because outgoing queue is full for connection id %u", conn->id());
            return nullptr;
        }
    }
    m_incoming_procuder_header->conn = conn;
    m_incoming_procuder_header->size = 0;

    capacity = (m_incoming_queue.element_size() - QUEUE_ENTRY_HEAD_SIZE);

    return m_incoming_procuder_header->data;
}

void* bridge::reserve_incoming_buffer(std::size_t capacity)
{
    capacity += QUEUE_ENTRY_HEAD_SIZE;

    Check_NotNullPtr(m_incoming_procuder_header, nullptr);

    connection* conn = m_incoming_procuder_header->conn;

    const size_t new_stride = std::exchange(capacity, m_incoming_queue.element_size());

    if(m_incoming_queue.change_element_size(new_stride, reinterpret_cast<void**>(&m_incoming_procuder_header)))
    {
        LOGV_("Comm bridge increased incoming queue stride from %zu to %zu for connection id %u", capacity, new_stride, conn->id());
        return m_incoming_procuder_header->data;
    }
    else
    {
        LOGW_("Comm bridge failed to increase incoming queue stride to %zu for connection id %u", new_stride, conn->id());

        Check_NotNullPtr(m_end_point, nullptr);
        m_end_point->on_error(error_received_too_large_data, nullptr, capacity-QUEUE_ENTRY_HEAD_SIZE);
        return nullptr;
    }
}

bool bridge::release_incoming_buffer(std::size_t size)
{
    Check_NotNullPtr(m_incoming_procuder_header, false);

    if(size == 0)
    {
        m_incoming_queue.cancel_producer_element_ownership(m_incoming_procuder_header);
        return true;
    }
    if((size + QUEUE_ENTRY_HEAD_SIZE) > m_incoming_queue.element_size())
    {
        m_incoming_queue.cancel_producer_element_ownership(m_incoming_procuder_header);
        Check_Arg_Invalid(size, false);
        return false;
    }

    connection* conn = m_incoming_procuder_header->conn;

    m_incoming_procuder_header->size = size;
    m_incoming_queue.release_producer_element_ownership(m_incoming_procuder_header);

    Check_NotNullPtr(m_end_point, nullptr);
    m_end_point->on_data_received(conn);

    return true;
}

bool bridge::have_sent_data(connection* conn)
{
    Check_Arg_NotNull(conn, nullptr);

    Check_NotNullPtr(m_service, nullptr);
    Check_ValidState(m_service->can_send_data(), nullptr);

    auto it = m_connections.find(conn);

    Check_ValidState(it != m_connections.end(), nullptr);

    queue_t& queue = it->queue;

    return !queue.empty();
}

void* bridge::acquire_sent_data(std::size_t& size, connection* conn)
{
    size = 0;

    Check_ValidState(m_outgoing_consumer_header == nullptr, nullptr);

    Check_NotNullPtr(m_service, nullptr);
    Check_ValidState(m_service->can_send_data(), nullptr);

    auto it = m_connections.find(conn);

    Check_ValidState(it != m_connections.end(), nullptr);

    queue_t& queue = it->queue;

    m_outgoing_consumer_header = queue.try_acquire_consumer_element_ownership<header_t>();

    if(m_outgoing_consumer_header == nullptr) return nullptr;

    Assert_Check(conn == m_outgoing_consumer_header->conn);

    size = m_outgoing_consumer_header->size;
    return m_outgoing_consumer_header->data;
}

bool bridge::release_sent_data(bool cancel)
{
    Check_ValidState(m_outgoing_consumer_header != nullptr, false);

    auto it = m_connections.find(m_outgoing_consumer_header->conn);

    Check_ValidState(it != m_connections.end(), nullptr);

    queue_t& queue = it->queue;

    if(cancel)
    {
        return queue.cancel_consumer_element_ownership(m_outgoing_consumer_header);
    }
    else
    {
        return queue.release_consumer_element_ownership(m_outgoing_consumer_header);
    }
}

}
}
