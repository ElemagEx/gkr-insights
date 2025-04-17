#pragma once

#include <gkr/comm/api.hpp>

#include <gkr/container/lockfree_bag.hpp>
#include <gkr/container/lockfree_queue.hpp>

namespace gkr
{
class params;

namespace comm
{

class connection;
class end_point;
class service;
class bridge
{
    bridge(      bridge&&) noexcept = delete;
    bridge(const bridge& ) noexcept = delete;

    bridge& operator=(      bridge&&) noexcept = delete;
    bridge& operator=(const bridge& ) noexcept = delete;

    bridge() noexcept = delete;

    struct header_t
    {
        connection*  conn;
        std::size_t  size;
        char         data[1];
    };

public:
    enum
    {
        // service to bridge
        event_init,
        event_pong,
        event_done,
        event_leave,

        event_connect,
        event_disconnect,
        event_failed_connect,

        // bridge to end-point
        error_connection_failed,
        error_outgoing_queue_maxed,
        error_incoming_queue_maxed,
        error_received_too_large_data,
        error_received_unexpected_data,
    };

public:
    using queue_t = lockfree_queue<void, false, true>;

    static constexpr float MINIMUM_RESERVE_FACTOR = 1.01f;
    static constexpr float MAXIMUM_RESERVE_FACTOR = 2.50f;

    static constexpr std::size_t QUEUE_ENTRY_HEAD_SIZE = offsetof(header_t, data);

private:
    struct slot_t
    {
        bool operator==(connection* c) const noexcept { return (c == conn); }
        connection* conn;
        queue_t     queue;
    };
    using bag_t = lockfree_bag<slot_t>;

    bag_t       m_connections;
    queue_t     m_incoming_queue;

    header_t*   m_incoming_procuder_header = nullptr;
    header_t*   m_incoming_consumer_header = nullptr;
    header_t*   m_outgoing_procuder_header = nullptr;
    header_t*   m_outgoing_consumer_header = nullptr;

    end_point*  m_end_point       = nullptr;
    service*    m_service         = nullptr;
    std::size_t m_incoming_maxcap = 64;
    std::size_t m_outgoing_maxcap = 64;
    std::size_t m_outgoing_count  = 16;
    std::size_t m_outgoing_size   = QUEUE_ENTRY_HEAD_SIZE;
    float       m_outgoing_factor = 1.5f;
    float       m_incoming_factor = 1.5f;

public:
    GKR_COMM_API  bridge(end_point* ep, service* s);
    GKR_COMM_API ~bridge();

public:
    //
    // called from end-point
    //
    GKR_COMM_API bool configure_outgoing_queue(std::size_t max_count, std::size_t init_count, std::size_t init_size, float reserve_factor);
    GKR_COMM_API bool configure_incoming_queue(std::size_t max_count, std::size_t init_count, std::size_t init_size, float reserve_factor);

    GKR_COMM_API bool connect();
    GKR_COMM_API bool listen();
    GKR_COMM_API void close(connection* conn = nullptr, int status = 0, const char* reason = nullptr);
    GKR_COMM_API void leave();

    GKR_COMM_API void* acquire_outgoing_buffer(std::size_t& capacity, connection* conn);
    GKR_COMM_API void* reserve_outgoing_buffer(std::size_t capacity);
    GKR_COMM_API bool  release_outgoing_buffer(std::size_t size);

    GKR_COMM_API bool have_received_data();

    GKR_COMM_API void* acquire_received_data(std::size_t& size, connection*& conn);
    GKR_COMM_API bool  release_received_data(bool cancel = false);

public:
    //
    // called from service
    //
    GKR_COMM_API void on_event(int event, void* data = nullptr, std::size_t size = 0);

    GKR_COMM_API void* acquire_incoming_buffer(std::size_t& capacity, connection* conn);
    GKR_COMM_API void* reserve_incoming_buffer(std::size_t capacity);
    GKR_COMM_API bool  release_incoming_buffer(std::size_t size);

    GKR_COMM_API bool have_sent_data(connection* conn);

    GKR_COMM_API void* acquire_sent_data(std::size_t& size, connection* conn);
    GKR_COMM_API bool  release_sent_data(bool cancel = false);
};

}
}
