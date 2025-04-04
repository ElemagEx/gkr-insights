#pragma once

#include <gkr/comm/client_registrator.hpp>

#include <gkr/comm/providers/lws_client.hpp>

#include <cstring>

extern "C"
{

struct gkr_comm_provider* gkr_comm_client_register_provider(const char* name)
{
    gkr::comm::provider* provider = nullptr;

    if((name == nullptr) || !std::strcmp(name, "libwebsocket"))
    {
        provider = gkr::comm::providers::libwebsocket::client::create();
    }

    if(provider == nullptr) return nullptr;

    return reinterpret_cast<struct gkr_comm_provider*>(gkr::comm::registry::register_provider(provider));
}

}
