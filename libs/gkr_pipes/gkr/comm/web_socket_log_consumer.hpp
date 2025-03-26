#pragma once

#include <gkr/log/consumer.hpp>

namespace gkr
{
namespace comm
{

class web_socket_log_consumer : public log::consumer
{
public:
    web_socket_log_consumer(
        const char* url,
        const char* provider = nullptr
        );
    virtual ~web_socket_log_consumer() override;

protected:
    virtual bool init_logging() override;
    virtual void done_logging() override;

    virtual bool  filter_log_message(const log::message& msg) override;
    virtual void consume_log_message(const log::message& msg) override;
};

}
}
