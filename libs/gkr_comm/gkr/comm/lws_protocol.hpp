#pragma once

#include <gkr/comm/api.hpp>

#include <utility>

struct lws;
enum lws_callback_reasons;

namespace libWebSocket
{

int main_protocol_callback(struct lws*, enum lws_callback_reasons, void*, void*, std::size_t);

class Protocol
{
    Protocol(const Protocol&) noexcept = delete;
    Protocol& operator=(const Protocol&) noexcept = delete;

public:
    GKR_COMM_API Protocol();
    GKR_COMM_API virtual ~Protocol();

    Protocol(Protocol&&) noexcept
    {
    }
    Protocol& operator=(Protocol&&) noexcept
    {
        return *this;
    }

public:
    virtual const char* get_info(unsigned& id, std::size_t& ps_size, std::size_t& rx_size, std::size_t& tx_size) = 0;

private:
    friend int main_protocol_callback(struct lws*, enum lws_callback_reasons, void*, void*, std::size_t);

protected:
    virtual void on_init() = 0;
    virtual void on_done() = 0;

    virtual void on_connection_opened() = 0;
    virtual void on_connection_closed() = 0;

    virtual void on_connection_received_data   () = 0;
    virtual void on_connection_client_writeable() = 0;
    virtual void on_connection_server_writeable() = 0;
};

}
