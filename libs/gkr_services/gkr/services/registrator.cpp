#pragma once

#include <gkr/comm/registry.hpp>
#include <gkr/services/lws_server.hpp>

#include <cstring>

extern "C"
{

struct gkr_comm_provider* gkr_comm_register_server_provider(const char* name)
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
