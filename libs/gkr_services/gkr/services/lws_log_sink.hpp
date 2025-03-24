#pragma once

#include <gkr/comm/lws_protocol.hpp>

namespace gkr
{
namespace services
{
namespace lws
{

class LogSink : public libWebSocket::Protocol
{
    LogSink(const LogSink&) noexcept = delete;
    LogSink& operator=(const LogSink&) noexcept = delete;

public:
    LogSink();
    virtual ~LogSink();

    LogSink(LogSink&&) noexcept
    {
    }
    LogSink& operator=(LogSink&&) noexcept
    {
        return *this;
    }

protected:
    virtual const char* get_info(unsigned& id, std::size_t& ps_size, std::size_t& rx_size, std::size_t& tx_size) override;

protected:
    virtual void on_init() override;
    virtual void on_done() override;

    virtual void on_connection_opened() override;
    virtual void on_connection_closed() override;

    virtual void on_connection_received_data   () override;
    virtual void on_connection_client_writeable() override;
    virtual void on_connection_server_writeable() override;
};

}
}
}
