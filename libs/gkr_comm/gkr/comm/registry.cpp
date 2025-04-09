#include <gkr/defs.hpp>

#include <gkr/comm/registry.hpp>
#include <gkr/comm/providers/lws_context.hpp>
#include <gkr/comm/constants.hpp>

#include <gkr/comm/log.hxx>

#include <gkr/log/logging.hpp>
#include <gkr/diagnostics.hpp>

#include <vector>

void* gkr_comm_log_instance = nullptr;

namespace gkr
{
namespace comm
{

provider::~provider()
{
}

class storage
{
    storage           (const storage&) noexcept = delete;
    storage& operator=(const storage&) noexcept = delete;

    storage           (storage&&) noexcept = delete;
    storage& operator=(storage&&) noexcept = delete;

private:
    std::vector<provider*> m_providers;

    bool m_initialized = false;
    bool m_started     = false;

public:
    storage() noexcept
    {
    }
    ~storage()
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

        m_initialized = true;
    }
    void done()
    {
        m_initialized = false;

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

        if((name == nullptr) || (*name == 0)) return m_providers.front();

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

        LOGI_("Comm provider registed: %s", provider->get_name());

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
            if(!m_providers[index]->start())
            {
                LOGE_("Failed to start provider: %s", m_providers[index]->get_name());
                break;
            }
            LOGV_("Comm provider started: %s", m_providers[index]->get_name());
        }
        if(index == m_providers.size())
        {
            m_started = true;
            LOGI("All comm providers are started");
        }
        else for( ; index > 0; --index)
        {
            m_providers[index - 1]->stop();
            LOGV_("Comm provider stopped: %s", m_providers[index - 1]->get_name());
        }
        return m_started;
    }
    bool stop()
    {
        if(!is_started()) return false;

        for(std::size_t index = m_providers.size() ; index > 0; --index)
        {
            m_providers[index - 1]->stop();
            LOGV_("Comm provider stopped: %s", m_providers[index - 1]->get_name());
        }
        m_started = false;
        LOGI("All comm providers are stopped");
        return true;
    }
};

}
}

namespace
{
gkr::comm::storage& get_storage()
{
    static gkr::comm::storage s_storage;
    return s_storage;
}
}

extern "C"
{

int gkr_comm_providers_registry_init(int clients_only, int add_log_comm_channel)
{
    Check_ValidState(!get_storage().is_initialized(), gkr_b2i(false));

    get_storage().init(clients_only);

    if(add_log_comm_channel)
    {
        gkr_log_name_id_pair severities_infos[] = LOG_SEVERITIES_INFOS;
        gkr_log_name_id_pair facilities_infos[] = {
            {"gkr-comm", COMM_LOG_FACILITY_LIBRARY},
            {"libWS"   , COMM_LOG_FACILITY_LWS    },
            {nullptr  , 0}
        };
        gkr_comm_log_instance = gkr_log_add_channel(COMM_LOG_CHANNEL_NAME, 16, 256, severities_infos, facilities_infos);
    }
    return gkr_b2i(true);
}

int gkr_comm_providers_registry_done()
{
    if(!get_storage().is_initialized()) return gkr_b2i(false);

    get_storage().done();
    return gkr_b2i(true);
}

int gkr_comm_providers_count_in_registry()
{
    Check_ValidState(get_storage().is_initialized(), 0);

    return int(get_storage().count());
}

struct gkr_comm_provider* gkr_comm_providers_get_from_registry(int index)
{
    Check_Arg_IsValid(std::size_t(index) <= get_storage().count(), nullptr);

    Check_ValidState(get_storage().is_initialized(), nullptr);

    return reinterpret_cast<struct gkr_comm_provider*>(get_storage().get(std::size_t(index)));
}

struct gkr_comm_provider* gkr_comm_providers_find_in_registry(const char* name)
{
    Check_ValidState(get_storage().is_initialized(), nullptr);

    return reinterpret_cast<struct gkr_comm_provider*>(get_storage().find(name));
}

int gkr_comm_providers_start_all()
{
    Check_ValidState( get_storage().is_initialized(), gkr_b2i(false));
    Check_ValidState(!get_storage().is_started    (), gkr_b2i(false));
    Check_ValidState(!get_storage().is_empty      (), gkr_b2i(false));

    return gkr_b2i(get_storage().start());
}

int gkr_comm_providers_stop_all()
{
    Check_ValidState(get_storage().is_initialized(), gkr_b2i(false));

    return gkr_b2i(get_storage().stop());
}

const char* gkr_comm_provider_get_name(struct gkr_comm_provider* provider)
{
    return reinterpret_cast<gkr::comm::provider*>(provider)->get_name();
}

struct gkr_comm_provider* gkr_comm_register_provider(const char* name)
{
    Check_ValidState(get_storage().is_initialized(), nullptr);

    gkr::comm::provider* provider = nullptr;

    if((name == nullptr) || !std::strcmp(name, "libwebsocket"))
    {
        provider = gkr::comm::providers::libwebsocket::context::create();
    }

    if(provider == nullptr) return nullptr;

    return reinterpret_cast<struct gkr_comm_provider*>(get_storage().add(provider));
}

}
