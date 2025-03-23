#pragma once

#include "lws_context.hpp"

namespace gkr
{
namespace mediator
{
namespace lws
{

using Protocol = libWebSocket::Protocol;

class Server : public libWebSocket::Context
{
    Server(const Server&) noexcept = delete;
    Server& operator=(const Server&) noexcept = delete;

public:
    Server() noexcept;
    virtual ~Server() noexcept;

    Server(Server&& other) noexcept : Context(std::move(other))
    {
    }
    Server& operator=(Server&& other) noexcept
    {
        Context::operator=(std::move(other));
        return *this;
    }

protected:
    virtual void get_context_info(unsigned& protocols, unsigned long long& options) override;

    virtual bool get_server_info(int& port, unsigned& http_dummy_protocol_index, const struct lws_http_mount*& mount) override;

    virtual Protocol* create_protocol(unsigned index) override;
};

}
}
}
