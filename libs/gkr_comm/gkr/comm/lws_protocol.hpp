#pragma once

#include <gkr/comm/api.hpp>

#include <gkr/url.hpp>

#include <utility>

struct lws;
struct lws_context;
enum lws_callback_reasons;

namespace gkr
{
namespace providers
{
namespace lws
{

class context;
class protocol
{
    friend class context;
    struct lws_context* m_context = nullptr;

protected:
    GKR_COMM_API protocol();

public:
    GKR_COMM_API virtual ~protocol();

private:
    void set_parent_context(struct lws_context* context)
    {
        m_context = context;
    }
protected:
    struct lws_context* get_parent_context()
    {
        return m_context;
    }

protected:
    virtual void* get_callback() = 0;

    virtual const char* get_name() = 0;

    virtual unsigned get_info(std::size_t& ps_size, std::size_t& rx_size, std::size_t& tx_size) = 0;

    virtual void on_other_reason(int reason, const void* data, std::size_t size) = 0;
};

class dummy_protocol : public protocol
{
public:
    GKR_COMM_API dummy_protocol();
    GKR_COMM_API virtual ~dummy_protocol() override;

protected:
    virtual void* get_callback() override;

    virtual const char* get_name() override;

    virtual unsigned get_info(std::size_t& ps_size, std::size_t& rx_size, std::size_t& tx_size) override;

    virtual void on_other_reason(int reason, const void* data, std::size_t size) override;

private:
    static int dummy_callback(struct lws*, enum lws_callback_reasons, void*, void*, std::size_t);
};

class server_protocol : public protocol
{
public:
    GKR_COMM_API server_protocol();
    GKR_COMM_API virtual ~server_protocol() override;

protected:
    GKR_COMM_API virtual void* get_callback() override;

    virtual const char* get_name() = 0;

    virtual unsigned get_info(std::size_t& ps_size, std::size_t& rx_size, std::size_t& tx_size) = 0;

    virtual void on_other_reason(int reason, const void* data, std::size_t size) = 0;

private:
    static int server_callback(struct lws*, enum lws_callback_reasons, void*, void*, std::size_t);

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
    GKR_COMM_API client_protocol();
    GKR_COMM_API virtual ~client_protocol() override;

public:
    bool connect(const char* url, const char* protocol);

protected:
    GKR_COMM_API virtual void* get_callback() override;

    virtual const char* get_name() = 0;

    virtual unsigned get_info(std::size_t& ps_size, std::size_t& rx_size, std::size_t& tx_size) = 0;

    virtual void on_other_reason(int reason, const void* data, std::size_t size) = 0;

private:
    static int client_callback(struct lws*, enum lws_callback_reasons, void*, void*, std::size_t);

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
