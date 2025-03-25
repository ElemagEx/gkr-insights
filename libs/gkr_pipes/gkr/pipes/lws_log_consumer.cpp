#include <gkr/defs.hpp>

#include <gkr/pipes/lws_log_consumer.hpp>

namespace gkr
{
namespace pipes
{

lws_log_consumer::lws_log_consumer()
{
}

lws_log_consumer::~lws_log_consumer()
{
}

bool lws_log_consumer::init_logging()
{
    return true;
}

void lws_log_consumer::done_logging()
{
}

bool lws_log_consumer::filter_log_message(const log::message& msg)
{
    return false;
}

void lws_log_consumer::consume_log_message(const log::message& msg)
{
}

}
}
