#pragma once

#include <gkr/comm/api.hpp>

#include <gkr/comm/service.hpp>

#include <gkr/url.hpp>

#include <utility>

struct lws;
struct lws_context;
enum lws_callback_reasons;

namespace gkr
{
namespace comm
{
namespace providers
{
namespace libwebsocket
{

class context;
class protocol : public service
{
    friend class context;
    struct lws_context* m_context = nullptr;
    bool m_is_secure = false;

protected:
    protocol() noexcept
    {
    }
    virtual ~protocol() noexcept
    {
    }

protected:
    struct lws_context* get_parent_context() noexcept
    {
        return m_context;
    }
    bool is_secure() noexcept
    {
        return m_is_secure;
    }

protected:
    virtual void* get_callback() noexcept = 0;

    virtual int get_listen_port() noexcept = 0;

    virtual const char* get_name() noexcept = 0;

    virtual unsigned get_info(std::size_t& ps_size, std::size_t& rx_size, std::size_t& tx_size) = 0;

    virtual void on_other_reason(int reason, const void* data, std::size_t size) = 0;
};

class dummy_protocol : public protocol
{
public:
    dummy_protocol() noexcept
    {
    }
    virtual ~dummy_protocol() noexcept override
    {
    }

protected:
    virtual void* get_callback() noexcept override;

    virtual int get_listen_port() noexcept override;

    virtual const char* get_name() noexcept override;

    virtual unsigned get_info(std::size_t& ps_size, std::size_t& rx_size, std::size_t& tx_size) override;

    virtual void on_other_reason(int reason, const void* data, std::size_t size) override;

private:
    friend int dummy_callback(struct lws*, enum lws_callback_reasons, void*, void*, std::size_t);

private:
    virtual bool can_connect() override;
    virtual bool can_listen() override;

private:
    virtual void connect() override;
    virtual bool listen() override;
    virtual void close() override;

    virtual void on_data_sent() override;
    virtual void on_error() override;
};

class server_protocol : public protocol
{
protected:
    server_protocol() noexcept
    {
    }
    virtual ~server_protocol() noexcept override
    {
    }

protected:
    virtual void* get_callback() noexcept override;

    virtual int get_listen_port() noexcept = 0;

    virtual const char* get_name() noexcept = 0;

    virtual unsigned get_info(std::size_t& ps_size, std::size_t& rx_size, std::size_t& tx_size) = 0;

    virtual void on_other_reason(int reason, const void* data, std::size_t size) = 0;

private:
    friend int server_callback(struct lws*, enum lws_callback_reasons, void*, void*, std::size_t);

protected:
    virtual void on_init() = 0;
    virtual void on_done() = 0;

    virtual void on_connection_opened() = 0;
    virtual void on_connection_closed() = 0;

    virtual void on_connection_pong() = 0;

    virtual void on_connection_writeable() = 0;
    virtual void on_connection_received_data(const void* data, std::size_t size) = 0;
};

class client_protocol : public protocol
{
    url m_connect_url;

public:
    client_protocol() noexcept
    {
    }
    virtual ~client_protocol() noexcept override
    {
    }

public:
    bool connect(const char* url, const char* protocol);

protected:
    virtual void* get_callback() noexcept override;

    virtual int get_listen_port() noexcept override;

    virtual const char* get_name() noexcept = 0;

    virtual unsigned get_info(std::size_t& ps_size, std::size_t& rx_size, std::size_t& tx_size) = 0;

    virtual void on_other_reason(int reason, const void* data, std::size_t size) = 0;

private:
    friend int client_callback(struct lws*, enum lws_callback_reasons, void*, void*, std::size_t);

protected:
    virtual void on_init() = 0;
    virtual void on_done() = 0;

    virtual void on_connection_error(const char* reason) = 0;

    virtual void on_connection_opened() = 0;
    virtual void on_connection_closed() = 0;

    virtual void on_connection_pong() = 0;

    virtual void on_connection_writeable() = 0;
    virtual void on_connection_received_data(const void* data, std::size_t size) = 0;
};

}
}
}
}
