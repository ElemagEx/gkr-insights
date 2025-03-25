#pragma once

#include <gkr/pipes/api.hpp>

#include <gkr/log/consumer.hpp>

namespace gkr
{
namespace pipes
{

class lws_log_consumer : public log::consumer
{
public:
    GKR_PIPES_API lws_log_consumer();
    GKR_PIPES_API virtual ~lws_log_consumer() override;

protected:
    virtual bool init_logging() override;
    virtual void done_logging() override;

    virtual bool  filter_log_message(const log::message& msg) override;
    virtual void consume_log_message(const log::message& msg) override;
};

}
}
