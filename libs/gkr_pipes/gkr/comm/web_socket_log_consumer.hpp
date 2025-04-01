#pragma once

#include <gkr/capi/comm/web_socket_log_consumer.h>

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
    static constexpr const char* SUPPORTED_PATHS[] = {
        "/binary/v0"
    };
    static constexpr const char* DEFAULT_PATH = SUPPORTED_PATHS[0];

public:
    GKR_CLIENT_COMM_API web_socket_log_consumer(
        params& parameters,
        std::size_t root = 0
        );
    GKR_CLIENT_COMM_API web_socket_log_consumer(
        const char* url,
        const char* provider_name = nullptr,
        params* parameters = nullptr,
        std::size_t root = 0
        );
    GKR_CLIENT_COMM_API virtual ~web_socket_log_consumer() override;

protected:
    virtual bool init_logging() override;
    virtual void done_logging() override;

    virtual bool  filter_log_message(const log::message& msg) override;
    virtual void consume_log_message(const log::message& msg) override;

protected:
    virtual const char* compose_output(const log::message& msg, unsigned* len, int flags);

protected:
    virtual bool on_error(int evt, void* data, std::size_t) override;
    virtual void on_connect() override;
    virtual void on_disconnect() override;
    virtual void on_data_received() override;

private:
    std::shared_ptr<bridge> m_bridge;
    url                     m_url;
    params*                 m_params = nullptr;
    std::size_t             m_root   = 0;
};

}
}
