#pragma once

#include <gkr/comm/api.hpp>

#include <gkr/comm/provider.hpp>

#include <thread>
#include <vector>

struct lws_context;
struct lws_protocols;
struct lws_context_creation_info;

namespace gkr
{
class params;
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

    const params*       m_params    = nullptr;
    std::size_t         m_root      = 0;

    struct lws_context* m_context   = nullptr;
    protocols_t         m_protocols;
    std::thread         m_thread;
    bool                m_running   = false;

public:
    static context* create(const params* parameters = nullptr, std::size_t root = 0);

protected:
    context(const params* parameters, std::size_t root);

public:
    virtual ~context();

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
    virtual void release() override;

public:
    virtual const char* get_name() override;

protected:
    virtual bool start() override;
    virtual void stop () override;

    virtual std::shared_ptr<bridge> create_bridge(end_point* ep, const char* service_name, const char* transport, int port) override;

private:
    protocol* find_protocol(const char* name, bool is_secure, int port);

    protocol* add_protocol(protocol* p, bool is_secure);

private:
    bool setup(struct lws_context_creation_info& info);
};

}
}
}
}
