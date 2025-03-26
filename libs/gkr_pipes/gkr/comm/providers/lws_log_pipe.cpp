#include <gkr/defs.hpp>

#include <gkr/comm/providers/lws_log_pipe.hpp>

namespace gkr
{
namespace providers
{
namespace lws
{

log_pipe::log_pipe()
{
}

log_pipe::~log_pipe()
{
}

const char* log_pipe::get_name()
{
    return "gkr-log-sink";
}

unsigned log_pipe::get_info(std::size_t& ps_size, std::size_t& rx_size, std::size_t& tx_size)
{
    ps_size = 32;
    rx_size = 2048;
    tx_size = 0;
    return 0;
}

void log_pipe::on_other_reason(int reason, const void* data, std::size_t size)
{
}

void log_pipe::on_init()
{
}

void log_pipe::on_done()
{
}

void log_pipe::on_connection_error(const char* reason)
{
}

void log_pipe::on_connection_opened()
{
}

void log_pipe::on_connection_closed()
{
}

void log_pipe::on_connection_pong()
{
}

void log_pipe::on_connection_writeable()
{
}

void log_pipe::on_connection_received_data(const void* data, std::size_t size)
{
}

}
}
}
