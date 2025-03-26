#include <gkr/defs.hpp>

#include <gkr/comm/web_socket_log_consumer.hpp>

namespace gkr
{
namespace comm
{

web_socket_log_consumer::web_socket_log_consumer(
    const char* url,
    const char* provider
    )
{
}

web_socket_log_consumer::~web_socket_log_consumer()
{
}

bool web_socket_log_consumer::init_logging()
{
    return true;
}

void web_socket_log_consumer::done_logging()
{
}

bool web_socket_log_consumer::filter_log_message(const log::message& msg)
{
    return false;
}

void web_socket_log_consumer::consume_log_message(const log::message& msg)
{
}

}
}
