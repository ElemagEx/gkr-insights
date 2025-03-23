#pragma once

#include <utility>
#include <thread>

struct lws_context;
struct lws_context_creation_info;
struct lws_http_mount;
struct lws_protocols;

namespace libWebSocket
{

class Protocol;

class Context
{
    Context           (const Context&) noexcept = delete;
    Context& operator=(const Context&) noexcept = delete;

    struct lws_context*   m_context   = nullptr;
    struct lws_protocols* m_protocols = nullptr;
    std::thread           m_thread;
    bool                  m_running   = false;

protected:
    Context() noexcept = default;

public:
    virtual ~Context() noexcept;

    Context(Context&& other) noexcept
        : m_context  (std::exchange(other.m_context  , nullptr))
        , m_protocols(std::exchange(other.m_protocols, nullptr))
        , m_thread   (std::move    (other.m_thread   ))
    {
    }
    Context& operator=(Context&& other) noexcept
    {
        m_context   = std::exchange(other.m_context  , nullptr);
        m_protocols = std::exchange(other.m_protocols, nullptr);
        m_thread    = std::move    (other.m_thread   );
        return *this;
    }

public:
    bool is_valid() const
    {
        return (m_context != nullptr);
    }
    bool is_running() const
    {
        return m_running;
    }

private:
    int  thread_init();
    void thread_loop();
    void thread_done();

public:
    bool run();

    bool stop();

protected:
    virtual void get_context_info(unsigned& protocols, unsigned long long& options) = 0;

    virtual bool get_server_info(int& port, unsigned& http_dummy_protocol_index, const struct lws_http_mount*& mount) = 0;

    virtual Protocol* create_protocol(unsigned index) = 0;
};

}
