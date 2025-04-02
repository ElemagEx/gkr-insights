#pragma once

#include <gkr/comm/api.hpp>

#include <gkr/container/lockfree_queue.hpp>

namespace gkr
{
class params;

namespace comm
{

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

    static constexpr float MINIMUM_RESERVE_FACTOR = 1.01f;
    static constexpr float MAXIMUM_RESERVE_FACTOR = 2.00f;

private:
    queue_t    m_outgoing_queue;
    queue_t    m_incoming_queue;
    header_t*  m_outgoing_header = nullptr;
    header_t*  m_incoming_header = nullptr;
    end_point* m_end_point       = nullptr;
    service*   m_service         = nullptr;
    float      m_outgoing_factor = 0.5f;
    float      m_incoming_factor = 0.5f;
    bool       m_connected       = false;

public:
    GKR_COMM_API  bridge(end_point* ep, service* s) noexcept;
    GKR_COMM_API ~bridge();

public:
    //
    // called from end-point
    //
    GKR_COMM_API void configure_outgoing_queue(std::size_t init_count, std::size_t init_size, float reserve_factor);
    GKR_COMM_API void configure_incoming_queue(std::size_t init_count, std::size_t init_size, float reserve_factor);

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
