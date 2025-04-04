#include <gkr/defs.hpp>

#include <gkr/comm/providers/lws_log_sink.hpp>

namespace gkr
{
namespace comm
{
namespace providers
{
namespace libwebsocket
{

log_sink::log_sink()
{
}

log_sink::~log_sink()
{
}

const char* log_sink::get_name()
{
    return NAME;
}

unsigned log_sink::get_info(std::size_t& ps_size, std::size_t& rx_size, std::size_t& tx_size)
{
    ps_size = 32;
    rx_size = 2048;
    tx_size = 0;
    return 0;
}

void log_sink::on_other_reason(int reason, const void* data, std::size_t size)
{
}

void log_sink::on_init()
{
}

void log_sink::on_done()
{
}

void log_sink::on_connection_opened()
{
}

void log_sink::on_connection_closed()
{
}

void log_sink::on_connection_pong()
{
}

void log_sink::on_connection_writeable()
{
}

void log_sink::on_connection_received_data(const void* data, std::size_t size)
{
}

bool log_sink::can_connect()
{
    return false;
}

bool log_sink::can_listen()
{
    return true;
}

void log_sink::connect()
{
}

void log_sink::listen()
{
}

void log_sink::close()
{
}

void log_sink::on_data_sent()
{
}

void log_sink::on_error()
{
}

}
}
}
}
