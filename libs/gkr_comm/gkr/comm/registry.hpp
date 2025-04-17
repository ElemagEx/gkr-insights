#pragma once

#include <gkr/capi/comm/registry.h>

#include <gkr/comm/provider.hpp>

namespace gkr
{
class params;

namespace comm
{

struct registry final
{
    const bool initialed;

    registry(bool add_comm_log_channel = true) : initialed(gkr_comm_providers_registry_init(gkr_b2i(add_comm_log_channel)))
    {
    }
    ~registry()
    {
        if(initialed) gkr_comm_providers_registry_done();
    }

public:
    static provider* find_provider(const char* name)
    {
        return reinterpret_cast<provider*>(gkr_comm_providers_find_in_registry(name));
    }

public:
    static bool start_all_providers()
    {
        return gkr_comm_providers_start_all();
    }
    static bool stop_all_providers()
    {
        return gkr_comm_providers_stop_all();
    }

public:
    static provider* register_provider(const char* name)
    {
        return reinterpret_cast<provider*>(gkr_comm_register_provider(name));
    }
    static provider* register_provider(const char* name, params& parameters, std::size_t root = 0)
    {
        return reinterpret_cast<provider*>(gkr_comm_register_provider_ex(name, reinterpret_cast<struct gkr_params*>(&parameters), root));
    }

private:
    registry           (const registry&) noexcept = delete;
    registry& operator=(const registry&) noexcept = delete;

    registry           (registry&&) noexcept = delete;
    registry& operator=(registry&&) noexcept = delete;
};

}
}
