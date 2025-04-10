#pragma once

#include <gkr/comm/providers/lws_protocol.hpp>

namespace gkr
{
namespace comm
{
namespace providers
{
namespace libwebsocket
{

class log_sink : public server_protocol
{
    int m_port = 0;

public:
    static constexpr const char* NAME = "gkr-log-sink";

public:
    log_sink(int port) noexcept : m_port(port)
    {
    }
    virtual ~log_sink() noexcept override
    {
    }

protected:
    virtual int get_listen_port() noexcept override;

    virtual const char* get_name() noexcept override;

    virtual unsigned get_info(std::size_t& ps_size, std::size_t& rx_size, std::size_t& tx_size) override;

    virtual void on_other_reason(int reason, const void* data, std::size_t size) override;

protected:
    virtual void on_init() override;
    virtual void on_done() override;

    virtual void on_connection_opened() override;
    virtual void on_connection_closed() override;

    virtual void on_connection_pong() override;

    virtual void on_connection_writeable() override;
    virtual void on_connection_received_data(const void* data, std::size_t size) override;

protected:
    virtual bool can_connect() override;
    virtual bool can_listen() override;

protected:
    virtual void connect() override;
    virtual bool listen() override;
    virtual void close() override;

    virtual void on_data_sent() override;
    virtual void on_error() override;
};

}
}
}
}
