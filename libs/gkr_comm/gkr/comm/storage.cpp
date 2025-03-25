#include <gkr/defs.hpp>

#include <gkr/comm/storage.hpp>

#include <gkr/comm/provider.hpp>
#include <gkr/diagnostics.hpp>

namespace gkr
{
namespace providers
{

provider::~provider()
{
}

storage::storage(bool client_only) : m_client_only(client_only)
{
    m_providers.reserve(4);
}

storage::~storage()
{
    for(std::size_t index = 0; index < m_providers.size(); ++index)
    {
        m_providers[index]->release();
    }
}

provider* storage::get_provider(std::size_t index)
{
    Check_Arg_IsValid(index <= m_providers.size(), nullptr);

    if(index >= m_providers.size()) return nullptr;

    return m_providers[index];
}

provider* storage::get_provider(const char* name)
{
    if(m_providers.empty()) return nullptr;

    if(name == nullptr) return m_providers.front();

    for(std::size_t index = 0; index < m_providers.size(); ++index)
    {
        if(!std::strcmp(name, m_providers[index]->get_name())) return m_providers[index];
    }
    return nullptr;
}

bool storage::add_provider(provider* provider)
{
    Check_Arg_NotNull(provider, false);

    Check_ValidState(!m_started, false);

    Check_ValidState(get_provider(provider->get_name()) == nullptr, nullptr);

    m_providers.push_back(provider);

    return true;
}

bool storage::start()
{
    Check_ValidState(!m_started, false);

    std::size_t index = 0;

    for( ; index < m_providers.size(); ++index)
    {
        if(!m_providers[index]->start()) break;
    }
    if(index == m_providers.size())
    {
        m_started = true;
    }
    else for( ; index > 0; --index)
    {
        m_providers[index - 1]->stop();
    }
    return m_started;
}

bool storage::stop()
{
    Check_ValidState(m_started, false);

    for(std::size_t index = m_providers.size() ; index > 0; --index)
    {
        m_providers[index - 1]->stop();
    }
    return true;
}

}
}
