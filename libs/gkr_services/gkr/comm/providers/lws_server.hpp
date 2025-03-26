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

class server : public context
{
public:
    static server* create();

protected:
    server() noexcept;
    virtual ~server() noexcept override;

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
}
