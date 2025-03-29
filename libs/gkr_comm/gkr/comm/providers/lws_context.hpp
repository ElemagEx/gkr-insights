#pragma once

#include <gkr/comm/api.hpp>

#include <gkr/comm/provider.hpp>

#include <thread>
#include <vector>

struct lws_context;
struct lws_context_creation_info;
struct lws_http_mount;
struct lws_protocols;

namespace gkr
{
namespace comm
{
namespace providers
{
namespace libwebsocket
{

class protocol;
class context : public provider
{
    using protocols_t = std::vector<struct lws_protocols>;

    struct lws_context* m_context   = nullptr;
    protocols_t         m_protocols;
    std::thread         m_thread;
    bool                m_running   = false;

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

protected:
    virtual void release() = 0;

public:
    GKR_COMM_API virtual const char* get_name() override;

protected:
    GKR_COMM_API virtual bool start() override;
    GKR_COMM_API virtual void stop () override;

protected:
    GKR_COMM_API protocol* find_protocol(const char* name);

    GKR_COMM_API void add_protocol(protocol* p);

protected:
    virtual void get_context_info(unsigned long long& options) = 0;

    virtual bool get_server_info(int& port, const struct lws_http_mount*& mount) = 0;
};

}
}
}
}
