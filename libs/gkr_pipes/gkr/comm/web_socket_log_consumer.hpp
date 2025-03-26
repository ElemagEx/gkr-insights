#pragma once

#include <gkr/log/consumer.hpp>

namespace gkr
{
namespace comm
{

class lws_log_consumer : public log::consumer
{
public:
    lws_log_consumer();
    virtual ~lws_log_consumer() override;

protected:
    virtual bool init_logging() override;
    virtual void done_logging() override;

    virtual bool  filter_log_message(const log::message& msg) override;
    virtual void consume_log_message(const log::message& msg) override;
};

}
}
