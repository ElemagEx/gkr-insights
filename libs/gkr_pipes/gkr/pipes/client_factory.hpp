#pragma once

#include <gkr/pipes/api.hpp>

#include <gkr/comm/provider.hpp>

namespace gkr
{
namespace providers
{

GKR_PIPES_API provider* create_client_provider(const char* name = nullptr);

}
}
