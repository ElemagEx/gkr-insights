#include <gkr/defs.hpp>

#include <gkr/comm/registry.hpp>

#include <gkr/diagnostics.hpp>

#include <vector>

namespace gkr
{
namespace comm
{

provider::~provider()
{
}

class providers
{
    providers           (const providers&) noexcept = delete;
    providers& operator=(const providers&) noexcept = delete;

    providers           (providers&&) noexcept = delete;
    providers& operator=(providers&&) noexcept = delete;

private:
    std::vector<provider*> m_providers;

    bool m_initialized  = false;
    bool m_clients_only = false;
    bool m_started      = false;

public:
    providers() noexcept
    {
    }
    ~providers()
    {
        done();
    }

public:
    bool is_initialized() const noexcept
    {
        return m_initialized;
    }
    void init(bool clients_only)
    {
        m_providers.reserve(4);

        m_clients_only = clients_only;
        m_initialized  = true;
    }
    void done()
    {
        m_initialized  = false;
        m_clients_only = false;

        if(m_started)
        {
            stop();
        }
        for(std::size_t index = m_providers.size(); index > 0; --index)
        {
            m_providers[index - 1]->release();
        }
        m_providers.clear();
    }

public:
    bool is_empty() const noexcept
    {
        return m_providers.empty();
    }
    std::size_t count() const noexcept
    {
        return m_providers.size();
    }
    provider* get(std::size_t index) noexcept
    {
        if(index >= m_providers.size()) return nullptr;

        return m_providers[index];
    }
    provider* find(const char* name) noexcept
    {
        if(m_providers.empty()) return nullptr;

        if(name == nullptr) return m_providers.front();

        for(std::size_t index = 0; index < m_providers.size(); ++index)
        {
            if(!std::strcmp(name, m_providers[index]->get_name())) return m_providers[index];
        }
        return nullptr;
    }
    provider* add(provider* provider)
    {
        Check_Arg_NotNull(provider, nullptr);

        Check_ValidState(!is_started(), nullptr);

        Check_ValidState(find(provider->get_name()) == nullptr, nullptr);

        m_providers.push_back(provider);

        return provider;
    }

public:
    bool is_started() const
    {
        return m_started;
    }
    bool start()
    {
        if(is_started()) return false;
        if(is_empty  ()) return false;

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
    bool stop()
    {
        if(!is_started()) return false;

        for(std::size_t index = m_providers.size() ; index > 0; --index)
        {
            m_providers[index - 1]->stop();
        }
        m_started = false;
        return true;
    }
};

}
}

namespace
{
gkr::comm::providers& get_providers()
{
    static gkr::comm::providers s_providers;
    return s_providers;
}
}

extern "C"
{

int gkr_comm_providers_registry_init(int clients_only)
{
    Check_ValidState(!get_providers().is_initialized(), gkr_b2i(false));

    get_providers().init(clients_only);
    return gkr_b2i(true);
}

int gkr_comm_providers_registry_done()
{
    if(!get_providers().is_initialized()) return gkr_b2i(false);

    get_providers().done();
    return gkr_b2i(true);
}

int gkr_comm_providers_count_in_registry()
{
    Check_ValidState(get_providers().is_initialized(), 0);

    return int(get_providers().count());
}

struct gkr_comm_provider* gkr_comm_providers_get_from_registry(int index)
{
    Check_Arg_IsValid(std::size_t(index) <= get_providers().count(), nullptr);

    Check_ValidState(get_providers().is_initialized(), nullptr);

    return reinterpret_cast<struct gkr_comm_provider*>(get_providers().get(std::size_t(index)));
}

struct gkr_comm_provider* gkr_comm_providers_find_in_registry(const char* name)
{
    Check_ValidState(get_providers().is_initialized(), nullptr);

    return reinterpret_cast<struct gkr_comm_provider*>(get_providers().find(name));
}

int gkr_comm_providers_start_all()
{
    Check_ValidState( get_providers().is_initialized(), gkr_b2i(false));
    Check_ValidState(!get_providers().is_started    (), gkr_b2i(false));
    Check_ValidState(!get_providers().is_empty      (), gkr_b2i(false));

    return gkr_b2i(get_providers().start());
}

int gkr_comm_providers_stop_all()
{
    Check_ValidState(get_providers().is_initialized(), gkr_b2i(false));

    return gkr_b2i(get_providers().stop());
}

const char* gkr_comm_provider_get_name(struct gkr_comm_provider* provider)
{
    return reinterpret_cast<gkr::comm::provider*>(provider)->get_name();
}

}

namespace gkr
{
namespace comm
{
provider* registry::register_provider(provider* p)
{
    Check_ValidState(get_providers().is_initialized(), gkr_b2i(false));

    return get_providers().add(p);
}

}
}
