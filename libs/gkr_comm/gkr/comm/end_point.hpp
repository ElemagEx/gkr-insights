#pragma once

#include <gkr/comm/api.hpp>

#include <cstddef>

namespace gkr
{
namespace comm
{

class connection;
class end_point
{
protected:
    GKR_COMM_API virtual ~end_point();

public:
    virtual void on_error(int error, const void* data, std::size_t size) = 0;

    virtual void on_connect(connection* conn) = 0;
    virtual void on_disconnect(connection* conn) = 0;

    virtual void on_data_received(connection* conn) = 0;
};

}
}
