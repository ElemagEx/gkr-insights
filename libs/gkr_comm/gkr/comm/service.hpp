#pragma once

#include <gkr/comm/api.hpp>

namespace gkr
{
namespace comm
{

class connection;
class service
{
public:
    enum : int
    {
        FLAG_IS_CLIENT        = 0x00,
        FLAG_IS_SERVER        = 0x01,
        FLAG_ROLE_MASK        = 0x01,

        FLAG_SECURE           = 0x02,

        FLAG_CAN_SEND_DATA    = 0x04,
        FLAG_CAN_RECEIVE_DATA = 0x08,
    };

protected:
    int m_flags = 0;

    GKR_COMM_API service(int flags = 0) noexcept : m_flags(flags)
    {
    }

public:
    bool is_client() const noexcept
    {
        return ((m_flags & FLAG_ROLE_MASK) == FLAG_IS_CLIENT);
    }
    bool is_server() const noexcept
    {
        return ((m_flags & FLAG_ROLE_MASK) == FLAG_IS_SERVER);
    }
    bool is_secure() const noexcept
    {
        return ((m_flags & FLAG_SECURE) != 0);
    }
    bool can_send_data() const noexcept
    {
        return ((m_flags & FLAG_CAN_SEND_DATA) != 0);
    }
    bool can_receive_data() const noexcept
    {
        return ((m_flags & FLAG_CAN_RECEIVE_DATA) != 0);
    }

protected:
    GKR_COMM_API virtual ~service();

public:
    virtual bool connect() = 0;
    virtual bool listen() = 0;

    virtual void on_data_sent(connection* conn) = 0;

    virtual void close(connection* conn, int status, const char* reason) = 0;
};

}
}
