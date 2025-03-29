#pragma once

#include <gkr/comm/providers/lws_context.hpp>

namespace gkr
{
namespace comm
{
namespace providers
{
namespace libwebsocket
{

class client : public context
{
public:
    static client* create();

protected:
    client() noexcept;
    virtual ~client() noexcept override;

protected:
    virtual void release() override;

protected:
    virtual std::shared_ptr<bridge> create_bridge(const char* service_name, end_point* ep) override;

protected:
    virtual void get_context_info(unsigned long long& options) override;

    virtual bool get_server_info(int& port, const struct lws_http_mount*& mount) override;
};

}
}
}
}
