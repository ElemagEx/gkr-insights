#include <gkr/defs.hpp>

#include <gkr/services/server.hpp>

#include <gkr/services/lws_server.hpp>

namespace gkr
{
namespace services
{

Server::Server()
{
}
Server::~Server()
{
    if(m_provider == nullptr) return;

    delete static_cast<lws::Server*>(m_provider);
}

bool Server::add_service_provider_lws()
{
    if(m_provider != nullptr) return false;
    m_provider = new lws::Server();
    return true;
}

bool Server::run()
{
    if(m_provider == nullptr) return false;

    return static_cast<lws::Server*>(m_provider)->run();
}

bool Server::stop()
{
    if(m_provider == nullptr) return false;

    return static_cast<lws::Server*>(m_provider)->stop();
}

}
}
