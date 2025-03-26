#pragma once

#include <gkr/comm/registry.hpp>
#include <gkr/comm/providers/lws_server.hpp>

#include <cstring>

extern "C"
{

struct gkr_comm_provider* gkr_comm_server_register_provider(const char* name)
{
    gkr::comm::provider* provider = nullptr;

    if((name == nullptr) || !std::strcmp(name, "libwebsocket"))
    {
        provider = gkr::providers::lws::server::create();
    }

    if(provider == nullptr) return nullptr;

    return reinterpret_cast<struct gkr_comm_provider*>(gkr::comm::registry::register_provider(provider));
}

}
