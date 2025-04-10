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
    virtual bool can_connect() = 0;
    virtual bool can_listen() = 0;

public:
    virtual void connect() = 0;
    virtual bool listen() = 0;
    virtual void close() = 0;

    virtual void on_data_sent() = 0;
    virtual void on_error() = 0;
};

}
}
