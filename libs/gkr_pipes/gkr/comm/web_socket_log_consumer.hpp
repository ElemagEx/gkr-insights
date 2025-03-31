#pragma once

#include <gkr/log/consumer.hpp>

#include <gkr/comm/end_point.hpp>
#include <gkr/comm/bridge.hpp>
#include <gkr/url.hpp>

namespace gkr
{
class params;

namespace comm
{

class web_socket_log_consumer
    : public log::consumer
    , public end_point
{
public:
    web_socket_log_consumer(
        const params& parameters
        );
    web_socket_log_consumer(
        const char* url,
        const char* provider_name = nullptr,
        const params* parameters = nullptr
        );
    virtual ~web_socket_log_consumer() override;

protected:
    virtual bool init_logging() override;
    virtual void done_logging() override;

    virtual bool  filter_log_message(const log::message& msg) override;
    virtual void consume_log_message(const log::message& msg) override;

protected:
    bool on_error(int evt, void* data, std::size_t) override;
    void on_connect() override;
    void on_disconnect() override;
    void on_data_received() override;

private:
    std::shared_ptr<bridge> m_bridge;
    url                     m_url;
};

}
}
