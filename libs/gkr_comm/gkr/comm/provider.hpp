#pragma once

#include <gkr/comm/api.hpp>

namespace gkr
{
namespace providers
{

class provider
{
protected:
    GKR_COMM_API virtual ~provider();

public:
    virtual void release() = 0;

    virtual const char* get_name() = 0;

    virtual bool start() = 0;
    virtual void stop () = 0;
};

}
}
