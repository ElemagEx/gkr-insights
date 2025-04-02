#include <gkr/defs.hpp>

#include <gkr/comm/bridge.hpp>

#include <gkr/comm/service.hpp>
#include <gkr/comm/end_point.hpp>

namespace gkr
{
namespace comm
{

end_point::~end_point()
{
}
service::~service()
{
}

bridge::bridge(end_point* ep, service* s) noexcept
    : m_end_point(ep)
    , m_service  (s )
{
}

bridge::~bridge()
{
}

void bridge::configure_outgoing_queue(std::size_t init_count, std::size_t init_size, float reserve_factor)
{
    if(reserve_factor < MINIMUM_RESERVE_FACTOR) reserve_factor = MINIMUM_RESERVE_FACTOR;
    if(reserve_factor > MAXIMUM_RESERVE_FACTOR) reserve_factor = MAXIMUM_RESERVE_FACTOR;
    m_outgoing_factor = reserve_factor;

    m_outgoing_queue.reset(init_count, init_size);
    m_outgoing_queue.threading.set_current_thread_as_exclusive_producer();
}

void bridge::configure_incoming_queue(std::size_t init_count, std::size_t init_size, float reserve_factor)
{
    if(reserve_factor < MINIMUM_RESERVE_FACTOR) reserve_factor = MINIMUM_RESERVE_FACTOR;
    if(reserve_factor > MAXIMUM_RESERVE_FACTOR) reserve_factor = MAXIMUM_RESERVE_FACTOR;
    m_outgoing_factor = reserve_factor;
 
    m_outgoing_queue.reset(init_count, init_size);
    m_outgoing_queue.threading.set_current_thread_as_exclusive_consumer();
}

void bridge::connect()
{
}

void bridge::listen()
{
}

void bridge::close()
{
}

void bridge::done()
{
}

bool bridge::is_connected()
{
    return m_connected;
}

void* bridge::acquire_outgoing_buffer(std::size_t& capacity)
{
    Check_ValidState(m_outgoing_header == nullptr, nullptr);

    capacity = 0;

    if(!m_connected) return nullptr;

    m_outgoing_header = m_outgoing_queue.try_acquire_producer_element_ownership<header_t>();

    if(m_outgoing_header == nullptr)
    {
        bool reserve = false;
        {
            auto element = m_outgoing_queue.try_start_pop();

            if(element.pop_in_progress())
            {
                header_t& header = element.as<header_t>();

                Check_NotNullPtr(m_end_point, nullptr);
                reserve = m_end_point->on_error(event_outgoing_queue_full, header.data, header.size);

                if(reserve) element.cancel_pop();
            }
        }
        if(reserve)
        {
            const
            std::size_t old_capacity = m_outgoing_queue.capacity();
            std::size_t new_capacity = std::size_t(float(old_capacity) * m_outgoing_factor);

            if(new_capacity <= old_capacity) new_capacity = (new_capacity + 1);

            m_outgoing_queue.reserve(new_capacity);
        }
        m_outgoing_header = m_outgoing_queue.try_acquire_producer_element_ownership<header_t>();

        if(m_outgoing_header == nullptr) return nullptr;
    }
    capacity = (m_outgoing_queue.element_size() - sizeof(header_t));

    return m_outgoing_header->data;
}

void* bridge::reserve_outgoing_buffer(std::size_t capacity)
{
    const std::size_t count_elements = (m_outgoing_header == nullptr) ? 0 : 1;

    if(!m_outgoing_queue.change_element_size(capacity, reinterpret_cast<void**>(&m_outgoing_header), count_elements)) return nullptr;

    return m_outgoing_header->data;
}

bool bridge::release_outgoing_buffer(std::size_t size)
{
    Check_NotNullPtr(m_outgoing_header, false);

    if((size == 0) || (size > (m_outgoing_queue.element_size() - sizeof(header_t))))
    {
        m_outgoing_queue.cancel_producer_element_ownership(m_outgoing_header);
        return false;
    }
    m_outgoing_header->size = size;
    m_outgoing_queue.release_producer_element_ownership(m_outgoing_header);

    Check_NotNullPtr(m_service, false);
    m_service->on_data_sent();
    return true;
}

void* bridge::acquire_received_data(std::size_t& size)
{
    Check_ValidState(m_incoming_header == nullptr, nullptr);

    size = 0;

    if(!m_connected) return nullptr;

    m_incoming_header = m_outgoing_queue.try_acquire_consumer_element_ownership<header_t>();

    if(m_incoming_header == nullptr) return nullptr;

    size = m_incoming_header->size;

    return m_outgoing_header->data;
}

bool bridge::release_received_data(bool cancel)
{
    Check_ValidState(m_incoming_header == nullptr, false);

    if(cancel)
    {
        return m_incoming_queue.cancel_consumer_element_ownership(m_incoming_header);
    }

    return m_incoming_queue.release_consumer_element_ownership(m_incoming_header);
}

void bridge::on_event(int event, const void* data, std::size_t size)
{
    //switch(event)
}

void* bridge::acquire_incoming_buffer(std::size_t& capacity)
{
    Check_ValidState(m_incoming_header == nullptr, nullptr);

    capacity = 0;

    if(!m_connected) return nullptr;

    m_incoming_header = m_incoming_queue.try_acquire_producer_element_ownership<header_t>();

    if(m_incoming_header == nullptr)
    {
        bool reserve = false;
        {
            auto element = m_incoming_queue.try_start_pop();

            if(element.pop_in_progress())
            {
                header_t& header = element.as<header_t>();

                Check_NotNullPtr(m_end_point, nullptr);
                reserve = m_end_point->on_error(event_incoming_queue_full, header.data, header.size);

                if(reserve) element.cancel_pop();
            }
        }
        if(reserve)
        {
            const
            std::size_t old_capacity = m_incoming_queue.capacity();
            std::size_t new_capacity = std::size_t(float(old_capacity) * m_incoming_factor);

            if(new_capacity <= old_capacity) new_capacity = (new_capacity + 1);

            m_incoming_queue.reserve(new_capacity);
        }
        m_incoming_header = m_incoming_queue.try_acquire_producer_element_ownership<header_t>();

        if(m_incoming_header == nullptr) return nullptr;
    }
    capacity = (m_incoming_queue.element_size() - sizeof(header_t));

    return m_incoming_header->data;
}

void* bridge::reserve_incoming_buffer(std::size_t capacity)
{
    const std::size_t count_elements = (m_incoming_header == nullptr) ? 0 : 1;

    if(!m_incoming_queue.change_element_size(capacity, reinterpret_cast<void**>(&m_incoming_header), count_elements)) return nullptr;

    return m_incoming_header->data;
}

bool bridge::release_incoming_buffer(std::size_t size)
{
    Check_NotNullPtr(m_incoming_header, false);

    if((size == 0) || (size > (m_outgoing_queue.element_size() - sizeof(header_t))))
    {
        m_incoming_queue.cancel_producer_element_ownership(m_incoming_header);
        return false;
    }
    m_incoming_header->size = size;
    m_incoming_queue.release_producer_element_ownership(m_incoming_header);

    Check_NotNullPtr(m_end_point, false);
    m_end_point->on_data_received();
    return true;
}

void* bridge::acquire_sent_data(std::size_t& size)
{
    Check_ValidState(m_outgoing_header == nullptr, nullptr);

    size = 0;

    if(!m_connected) return nullptr;

    m_outgoing_header = m_outgoing_queue.try_acquire_consumer_element_ownership<header_t>();

    if(m_outgoing_header == nullptr) return nullptr;

    size = m_outgoing_header->size;

    return m_outgoing_header->data;
}

bool bridge::release_sent_data(bool cancel)
{
    Check_ValidState(m_outgoing_header == nullptr, false);

    if(cancel)
    {
        m_outgoing_queue.cancel_consumer_element_ownership(m_outgoing_header);
    }

    return m_outgoing_queue.release_consumer_element_ownership(m_outgoing_header);
}

}
}
