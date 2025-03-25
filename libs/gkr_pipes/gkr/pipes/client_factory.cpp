#pragma once

#include <gkr/pipes/client_factory.hpp>

#include <gkr/pipes/lws_client.hpp>

#include <cstring>

namespace gkr
{
namespace providers
{

provider* create_client_provider(const char* name)
{
    if((name == nullptr) || !std::strcmp(name, "libwebsocket"))
    {
        return lws::client::create();
    }
    return nullptr;
}

}
}
