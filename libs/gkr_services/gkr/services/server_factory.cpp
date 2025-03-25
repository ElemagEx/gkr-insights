#pragma once

#include <gkr/services/server_factory.hpp>

#include <gkr/services/lws_server.hpp>

#include <cstring>

namespace gkr
{
namespace providers
{

provider* create_server_provider(const char* name)
{
    if((name == nullptr) || !std::strcmp(name, "libwebsocket"))
    {
        return lws::server::create();
    }
    return nullptr;
}

}
}
