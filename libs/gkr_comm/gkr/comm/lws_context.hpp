#pragma once

#include <gkr/comm/api.hpp>

#include <utility>
#include <thread>

struct lws_context;
struct lws_context_creation_info;
struct lws_http_mount;
struct lws_protocols;

namespace gkr
{
namespace lws
{

class protocol;
class context
{
    struct lws_context*   m_context   = nullptr;
    struct lws_protocols* m_protocols = nullptr;
    std::thread           m_thread;
    bool                  m_running   = false;

protected:
    GKR_COMM_API context() noexcept;

public:
    GKR_COMM_API virtual ~context() noexcept;

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
    GKR_COMM_API bool run();

    GKR_COMM_API bool stop();

protected:
    virtual void get_context_info(unsigned& protocols, unsigned long long& options) = 0;

    virtual bool get_server_info(int& port, const struct lws_http_mount*& mount) = 0;

    virtual protocol* create_protocol(unsigned index) = 0;
};

}
}
