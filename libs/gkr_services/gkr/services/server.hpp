#pragma once

#include <gkr/services/api.hpp>

#include <utility>

namespace gkr
{
namespace services
{

class Server
{
    Server(const Server&) noexcept = delete;
    Server& operator=(const Server&) noexcept = delete;

    void* m_provider = nullptr;

public:
    GKR_SERVICES_API Server();
    GKR_SERVICES_API ~Server();

    Server(Server&& other) noexcept : m_provider(std::exchange(other.m_provider, nullptr))
    {
    }
    Server& operator=(Server&& other) noexcept
    {
        m_provider = std::exchange(other.m_provider, nullptr);
        return *this;
    }

public:
    GKR_SERVICES_API bool add_service_provider_lws();

public:
    GKR_SERVICES_API bool run();
    GKR_SERVICES_API bool stop();
};

}
}
