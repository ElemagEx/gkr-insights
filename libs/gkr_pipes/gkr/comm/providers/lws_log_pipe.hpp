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

class log_pipe : public client_protocol
{
public:
    log_pipe();
    virtual ~log_pipe() override;

protected:
    virtual const char* get_name() override;

    virtual unsigned get_info(std::size_t& ps_size, std::size_t& rx_size, std::size_t& tx_size) override;

    virtual void on_other_reason(int reason, const void* data, std::size_t size) override;

protected:
    virtual void on_init() override;
    virtual void on_done() override;

    virtual void on_connection_error(const char* reason) override;

    virtual void on_connection_opened() override;
    virtual void on_connection_closed() override;

    virtual void on_connection_pong() override;

    virtual void on_connection_writeable() override;
    virtual void on_connection_received_data(const void* data, std::size_t size) override;
};

}
}
}
}
