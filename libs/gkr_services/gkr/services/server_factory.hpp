#pragma once

#include <gkr/services/api.hpp>

#include <gkr/comm/provider.hpp>

namespace gkr
{
namespace providers
{

GKR_SERVICES_API provider* create_server_provider(const char* name = nullptr);

}
}
