#pragma once

#include <gkr/comm/api.hpp>

#include <memory>

namespace gkr
{
namespace comm
{

class end_point;
class bridge;

using bridge_ptr_t = std::shared_ptr<bridge>;

class provider
{
protected:
    GKR_COMM_API virtual ~provider();

public:
    virtual void release() = 0;

    virtual const char* get_name() = 0;

    virtual bool start() = 0;
    virtual void stop () = 0;

    virtual bridge_ptr_t create_bridge(end_point* ep, const char* service_name, const char* transport, int port = 0) = 0;
};

}
}
