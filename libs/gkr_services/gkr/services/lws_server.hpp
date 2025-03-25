#pragma once

#include <gkr/comm/lws_context.hpp>

namespace gkr
{
namespace lws
{

class server : public context
{
//  server(const server&) noexcept = delete;
//  server& operator=(const server&) noexcept = delete;

public:
    server() noexcept;
    virtual ~server() noexcept override;

//  server(server&& other) noexcept : context(std::move(other))
//  {
//  }
//  server& operator=(server&& other) noexcept
//  {
//      server::operator=(std::move(other));
//      return *this;
//  }

protected:
    virtual void get_context_info(unsigned& protocols, unsigned long long& options) override;

    virtual bool get_server_info(int& port, const struct lws_http_mount*& mount) override;

    virtual protocol* create_protocol(unsigned index) override;
};

}
}
