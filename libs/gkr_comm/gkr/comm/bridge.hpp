#pragma once

#include <gkr/comm/api.hpp>

#include <gkr/container/lockfree_queue.hpp>

namespace gkr
{
namespace comm
{

class end_point;
class service;
class bridge
{
    bridge           (const bridge&) noexcept = delete;
    bridge& operator=(const bridge&) noexcept = delete;

    bridge() noexcept = delete;

    struct header_t
    {
        std::size_t size;
        char        data[0];
    };
    enum
    {
        event_outgoing_queue_full,
        event_incoming_queue_full,
    };

public:
    using queue_t = lockfree_queue<void, false, true>;

    bridge(bridge&& other) noexcept(
        std::is_nothrow_move_constructible<queue_t>::value
        )
        : m_outgoing_queue (std::move    (other.m_outgoing_queue))
        , m_incoming_queue (std::move    (other.m_incoming_queue))
        , m_outgoing_header(std::exchange(other.m_outgoing_header, nullptr))
        , m_incoming_header(std::exchange(other.m_incoming_header, nullptr))
        , m_end_point      (std::exchange(other.m_end_point      , nullptr))
        , m_service        (std::exchange(other.m_service        , nullptr))
        , m_connected      (std::exchange(other.m_connected      , false  ))
    {
    }
    bridge& operator=(bridge&& other) noexcept(
        std::is_nothrow_move_constructible<queue_t>::value
        )
    {
        m_outgoing_queue  = std::move    (other.m_outgoing_queue);
        m_incoming_queue  = std::move    (other.m_incoming_queue);
        m_outgoing_header = std::exchange(other.m_outgoing_header, nullptr);
        m_incoming_header = std::exchange(other.m_incoming_header, nullptr);
        m_end_point       = std::exchange(other.m_end_point     , nullptr);
        m_service         = std::exchange(other.m_service       , nullptr);
        m_connected       = std::exchange(other.m_connected     , false  );
        return *this;
    }

private:
    queue_t    m_outgoing_queue;
    queue_t    m_incoming_queue;
    header_t*  m_outgoing_header = nullptr;
    header_t*  m_incoming_header = nullptr;
    end_point* m_end_point       = nullptr;
    service*   m_service         = nullptr;
    bool       m_connected       = false;


public:
    GKR_COMM_API  bridge(end_point* ep, service* s) noexcept;
    GKR_COMM_API ~bridge();

public:
    //
    // called from end-point
    //
    GKR_COMM_API void connect();
    GKR_COMM_API void listen();
    GKR_COMM_API void close();
    GKR_COMM_API void done();

    GKR_COMM_API bool is_connected();

    GKR_COMM_API void* acquire_outgoing_buffer(std::size_t& capacity);
    GKR_COMM_API void* reserve_outgoing_buffer(std::size_t capacity);
    GKR_COMM_API bool  release_outgoing_buffer(std::size_t size);

    GKR_COMM_API void* acquire_received_data(std::size_t& size);
    GKR_COMM_API bool  release_received_data(bool cancel = false);

public:
    //
    // called from service
    //
    GKR_COMM_API void on_event(int event, const void* data, std::size_t size);

    GKR_COMM_API void* acquire_incoming_buffer(std::size_t& capacity);
    GKR_COMM_API void* reserve_incoming_buffer(std::size_t capacity);
    GKR_COMM_API bool  release_incoming_buffer(std::size_t size);

    GKR_COMM_API void* acquire_sent_data(std::size_t& size);
    GKR_COMM_API bool  release_sent_data(bool cancel = false);
};

}
}
