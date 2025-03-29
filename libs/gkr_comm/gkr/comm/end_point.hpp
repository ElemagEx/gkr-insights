#pragma once

#include <gkr/comm/api.hpp>

#include <cstddef>

namespace gkr
{
namespace comm
{

class end_point
{
protected:
    GKR_COMM_API virtual ~end_point();

public:
    virtual bool on_error(int evt, void* data, std::size_t size) = 0;
    virtual void on_connect() = 0;
    virtual void on_disconnect() = 0;
    virtual void on_data_received() = 0;
};

}
}
