#include <gkr/defs.hpp>

#include <gkr/services/lws_log_sink.hpp>

namespace gkr
{
namespace services
{
namespace lws
{

LogSink::LogSink()
{
}

LogSink::~LogSink()
{
}

const char* LogSink::get_info(unsigned& id, std::size_t& ps_size, std::size_t& rx_size, std::size_t& tx_size)
{
    id = 0;

    ps_size = 32;
    rx_size = 2048;
    tx_size = 0;

    return "gkr-log-sink";
}

void LogSink::on_init()
{
}

void LogSink::on_done()
{
}

void LogSink::on_connection_opened()
{
}

void LogSink::on_connection_closed()
{
}

void LogSink::on_connection_received_data()
{
}

void LogSink::on_connection_client_writeable()
{
}

void LogSink::on_connection_server_writeable()
{
}

}
}
}
