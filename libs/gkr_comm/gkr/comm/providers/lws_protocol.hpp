#pragma once

#include <gkr/comm/service.hpp>

#include <memory>

struct lws;
struct lws_context;
enum lws_callback_reasons;

namespace gkr
{
namespace comm
{
class bridge;
namespace providers
{
namespace libwebsocket
{

class context;
class instance;
class protocol : public service
{
    friend class context;
    struct lws_context* m_context = nullptr;

    const char* m_name;
    int         m_port;

protected:
    std::shared_ptr<bridge> m_bridge;

protected:
    protocol(const char* name, int flags) noexcept
        : service(flags & ~service::FLAG_IS_SERVER)
        , m_name (name)
        , m_port (-1)
    {
    }
    protocol(const char* name, int flags, int port) noexcept
        : service(flags |  service::FLAG_IS_SERVER)
        , m_name (name)
        , m_port (port)
    {
    }
    virtual ~protocol() noexcept;

protected:
    struct lws_context* get_parent_context() const noexcept
    {
        return m_context;
    }
    const char* get_name() const noexcept
    {
        return m_name;
    }
    int get_listen_port() const noexcept
    {
        return m_port;
    }

protected:
    virtual bool connect() override;
    virtual bool listen() override;

    virtual void on_data_sent(connection* conn) override;

    virtual void close(connection* conn, int status, const char* reason) override;

protected:
    virtual void* get_callback() noexcept = 0;

    virtual unsigned get_info(std::size_t& rx_size, std::size_t& tx_size) = 0;

protected:
    void on_init();
    void on_done();

    void on_connection_opened(instance* inst);
    void on_connection_closed(instance* inst);

    void on_connection_pong(instance* inst);

    void on_connection_writeable(instance* inst);
    void on_connection_received_data(instance* inst, const void* data, std::size_t size);
};

class dummy_protocol : public protocol
{
public:
    dummy_protocol() noexcept : protocol("http", 0)
    {
    }
    virtual ~dummy_protocol() noexcept override;

protected:
    virtual void* get_callback() noexcept override;

    virtual unsigned get_info(std::size_t& rx_size, std::size_t& tx_size) override;

private:
    friend int dummy_callback(struct lws*, enum lws_callback_reasons, void*, void*, std::size_t);

    void callback(int reason, const void* data, std::size_t size);
};

class server_protocol : public protocol
{
    int m_port = 0;

public:
    server_protocol(const char* name, int flags, int port) noexcept : protocol(name, flags, port)
    {
    }

protected:
    virtual ~server_protocol() noexcept override;

    virtual void* get_callback() noexcept override;

    virtual unsigned get_info(std::size_t& rx_size, std::size_t& tx_size) override;

private:
    friend int server_callback(struct lws*, enum lws_callback_reasons, void*, void*, std::size_t);
};

class client_protocol : public protocol
{
public:
    client_protocol(const char* name, int flags) noexcept : protocol(name, flags)
    {
    }
//  bool connect(const char* url, const char* protocol);

protected:
    virtual ~client_protocol() noexcept override;

    virtual void* get_callback() noexcept override;

    virtual unsigned get_info(std::size_t& rx_size, std::size_t& tx_size) override;

private:
    friend int client_callback(struct lws*, enum lws_callback_reasons, void*, void*, std::size_t);

    void on_connect_error(char* reason, std::size_t len);
};

}
}
}
}
