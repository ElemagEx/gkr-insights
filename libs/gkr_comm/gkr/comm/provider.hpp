#pragma once

#include <gkr/comm/api.hpp>

#include <memory>

namespace gkr
{
namespace comm
{

class end_point;
class bridge;
class provider
{
protected:
    GKR_COMM_API virtual ~provider();

public:
    virtual void release() = 0;

    virtual const char* get_name() = 0;

    virtual bool start() = 0;
    virtual void stop () = 0;

    virtual std::shared_ptr<bridge> create_bridge(const char* service_name, end_point* ep) = 0;
};

}
}
