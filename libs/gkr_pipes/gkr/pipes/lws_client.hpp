#pragma once

#include <gkr/comm/lws_context.hpp>

namespace gkr
{
namespace providers
{
namespace lws
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
    virtual void get_context_info(unsigned& protocols, unsigned long long& options) override;

    virtual bool get_server_info(int& port, const struct lws_http_mount*& mount) override;

    virtual protocol* create_protocol(unsigned index) override;
};

}
}
}
