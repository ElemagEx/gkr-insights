#pragma once

#include <gkr/capi/comm/registry.h>

#include <gkr/comm/provider.hpp>

namespace gkr
{
namespace comm
{

struct registry final
{
    const bool initialed;

    registry(bool client_only) : initialed(gkr_comm_providers_registry_init(client_only?1:0))
    {
    }
    ~registry()
    {
        if(initialed) gkr_comm_providers_registry_done();
    }

public:
    bool start_all()
    {
        return gkr_comm_providers_start_all();
    }
    bool stop_all()
    {
        return gkr_comm_providers_stop_all();
    }

public:
    provider* register_client_provider(const char* name)
    {
        return reinterpret_cast<provider*>(gkr_comm_register_client_provider(name));
    }
    provider* register_server_provider(const char* name)
    {
        return reinterpret_cast<provider*>(gkr_comm_register_server_provider(name));
    }

public:
    GKR_COMM_API static provider* register_provider(provider* p);

private:
    registry           (const registry&) noexcept = delete;
    registry& operator=(const registry&) noexcept = delete;

    registry           (registry&&) noexcept = delete;
    registry& operator=(registry&&) noexcept = delete;
};

}
}
