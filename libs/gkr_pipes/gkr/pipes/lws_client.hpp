#pragma once

#include <gkr/comm/lws_context.hpp>

namespace gkr
{
namespace lws
{

class client : public context
{
//  client(const client&) noexcept = delete;
//  client& operator=(const client&) noexcept = delete;

public:
    client() noexcept;
    virtual ~client() noexcept override;

//  client(client&& other) noexcept : context(std::move(other))
//  {
//  }
//  client& operator=(client&& other) noexcept
//  {
//      context::operator=(std::move(other));
//      return *this;
//  }

protected:
    virtual void get_context_info(unsigned& protocols, unsigned long long& options) override;

    virtual bool get_server_info(int& port, const struct lws_http_mount*& mount) override;

    virtual protocol* create_protocol(unsigned index) override;
};

}
}
