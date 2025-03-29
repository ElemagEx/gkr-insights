#pragma once

#include <gkr/comm/api.hpp>

namespace gkr
{
namespace comm
{

class service
{
protected:
    GKR_COMM_API virtual ~service();

public:
    virtual void connect() = 0;
    virtual void listen() = 0;
    virtual void close() = 0;

    virtual void on_data_sent() = 0;
    virtual void on_error() = 0;
};

}
}
