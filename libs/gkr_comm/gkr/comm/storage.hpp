#pragma once

#include <gkr/comm/api.hpp>

#include <vector>

namespace gkr
{
namespace providers
{

class provider;
class storage
{
    storage(const storage&) noexcept = delete;
    storage& operator=(const storage&) noexcept = delete;

    std::vector<provider*> m_providers;
    bool                   m_client_only = false;
    bool                   m_started     = false;

public:
    GKR_COMM_API  storage(bool client_only);
    GKR_COMM_API ~storage();

    storage(storage&& other) noexcept
        : m_providers  (std::move    (other.m_providers  ))
        , m_client_only(std::exchange(other.m_client_only, false))
        , m_started    (std::exchange(other.m_started    , false))
    {
    }
    storage& operator=(storage&& other) noexcept
    {
        m_providers   = std::move    (other.m_providers  );
        m_client_only = std::exchange(other.m_client_only, false);
        m_started     = std::exchange(other.m_started    , false);
        return *this;
    }

public:
    GKR_COMM_API provider* get_provider(std::size_t index);
    GKR_COMM_API provider* get_provider(const char* name);

    GKR_COMM_API bool add_provider(provider* provider);

public:
    GKR_COMM_API bool start();
    GKR_COMM_API bool stop();
};

}
}
