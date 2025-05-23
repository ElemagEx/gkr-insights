#pragma once

#include <gkr/capi/comm/log_upstream_log_consumer.h>

#include <gkr/log/consumer.hpp>

#include <gkr/comm/end_point.hpp>
#include <gkr/comm/bridge.hpp>
#include <gkr/url.hpp>

namespace gkr
{
class params;

namespace comm
{

class upstream_log_consumer
    : public log::consumer
    , public end_point
{
public:
    enum
    {
        STREAM_FORMAT_BINARY = 0x0,
        STREAM_VERSION_0     = 0x0,
    };
    static constexpr const char* SUPPORTED_FORMATS [] = {"binary"};
    static constexpr unsigned    FORMAT_MAX_VERSION[] = {0};

    static_assert(std::size(SUPPORTED_FORMATS) == std::size(FORMAT_MAX_VERSION), "Must have same size");

public:
    GKR_COMM_API upstream_log_consumer(
        const params& parameters,
        std::size_t root = 0
        );
    GKR_COMM_API upstream_log_consumer(
        const char* url,
        const char* transport = nullptr,
        const char* provider_name = nullptr,
        const params* parameters = nullptr,
        std::size_t root = 0
        );
    GKR_COMM_API virtual ~upstream_log_consumer() override;

private:
    void reset(const params* parameters, std::size_t root);

public:
    GKR_COMM_API bool configure(
        const params& parameters,
        std::size_t root = 0
        );
    GKR_COMM_API bool configure(
        const char* url,
        const char* transport = nullptr,
        const char* provider_name = nullptr,
        const params* parameters = nullptr,
        std::size_t root = 0
        );

public:
    bool is_successfully_configured() const noexcept
    {
        return bool(m_bridge);
    }

protected:
    virtual bool init_logging() override;
    virtual void done_logging() override;

    virtual bool  filter_log_message(const log::message& msg) override;
    virtual void consume_log_message(const log::message& msg) override;

protected:
    virtual const char* compose_output(const log::message& msg, unsigned* len, int flags);

protected:
    virtual void on_error(int event, const void* data, std::size_t) override;
    virtual void on_connect(connection* conn) override;
    virtual void on_disconnect(connection* conn) override;
    virtual void on_data_received(connection* conn) override;

private:
    bool parse_path(const char* path);

    void configure_bridge();

private:
    std::shared_ptr<bridge> m_bridge;
    url                     m_url;
    connection*             m_conn    = nullptr;
    const params*           m_params  = nullptr;
    std::size_t             m_root    = 0;
    unsigned                m_format  = 0;
    unsigned                m_version = 0;
    bool                    m_started = false;
};

}
}
