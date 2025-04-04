#pragma once

#include <gkr/comm/registry.hpp>

namespace gkr
{
namespace comm
{

inline provider* client_register_provider(const char* name)
{
    return reinterpret_cast<provider*>(gkr_comm_client_register_provider(name));
}

}
}
