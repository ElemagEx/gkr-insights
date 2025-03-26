#pragma once

#include <gkr/comm/registry.hpp>

namespace gkr
{
namespace comm
{

inline provider* server_register_provider(const char* name)
{
    return reinterpret_cast<provider*>(gkr_comm_server_register_provider(name));
}

}
}
