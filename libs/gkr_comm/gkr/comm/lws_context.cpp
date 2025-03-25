#include <gkr/defs.hpp>

#include <gkr/comm/lws_context.hpp>
#include <gkr/comm/lws_protocol.hpp>

#include <gkr/diagnostics.hpp>
#include <gkr/log/defs/generic_cdefs.hpp>

#include <libwebsockets.h>

#include <cstring>
#include <atomic>

namespace gkr
{
namespace providers
{
namespace lws
{

context::context() noexcept
{
}

context::~context() noexcept
{
}

int context::thread_init()
{
    struct lws_context_creation_info info;
    std::memset(&info, 0, sizeof(info));

    unsigned protocols = 0;
    get_context_info(protocols, info.options);

    get_server_info(info.port, info.mounts);

    m_protocols = static_cast<struct lws_protocols*>(std::malloc(sizeof(struct lws_protocols) * std::size_t(protocols + 1)));

    Check_NotNullPtr(m_protocols, -1);

    std::memset(m_protocols, 0, sizeof(struct lws_protocols) * std::size_t(protocols + 1));

    for(unsigned index = 0; index < protocols; ++index)
    {
        struct lws_protocols& proto = m_protocols[index];

        proto.user = create_protocol(index);

        Check_NotNullPtr(proto.user, -1);

        proto.callback = static_cast<lws_callback_function *>(static_cast<protocol*>(proto.user)->get_callback());
        proto.name     = static_cast<protocol*>(proto.user)->get_name();
        proto.id       = static_cast<protocol*>(proto.user)->get_info(
            proto.per_session_data_size,
            proto.rx_buffer_size,
            proto.tx_packet_size);
    }
    m_protocols[protocols] = LWS_PROTOCOL_LIST_TERM;

    info.user = this;

    m_context = lws_create_context(&info);

    if(m_context == nullptr) return -1;

    for(unsigned index = 0; index < protocols; ++index)
    {
        static_cast<protocol*>(m_protocols[index].user)->set_parent_context(m_context);
    }
    return +1;
}

void context::thread_loop()
{
	for(m_running = true; m_running; )
    {
		if(0 > lws_service(m_context, 0))
        {
            m_running = false;
        }
    }
}

void context::thread_done()
{
    if(m_protocols != nullptr)
    {
        for(unsigned index = 0; ; ++index)
        {
            struct lws_protocols& proto = m_protocols[index];

            if(proto.name == nullptr) break;
            if(proto.user == nullptr) continue;

            delete static_cast<protocol*>(proto.user);
        }
        std::free(m_protocols);
        m_protocols = nullptr;
    }
    if(m_context != nullptr)
    {
        lws_context_destroy(m_context);
        m_context = nullptr;
    }
}

const char* context::get_name()
{
    return "libwebsocket";
}

bool context::start()
{
    Check_ValidState(!m_thread.joinable(), false);

    lws_set_log_level(LLL_USER | LLL_ERR | LLL_WARN | LLL_NOTICE, nullptr);

    std::atomic_int initialized { 0 };

    m_thread = std::thread([this, &initialized] () {
        int result = thread_init();
        initialized.store(result, std::memory_order_release);
        if(result != -1) thread_loop();
        thread_done();
    });
    for( ; ; std::this_thread::yield())
    {
        int result = initialized.load(std::memory_order_relaxed);

        if(result ==  0) continue;
        if(result != -1) return true;

        m_thread.join();
        return false;
    }
}

void context::stop()
{
    Check_ValidState(m_thread.joinable(), );

    m_running = false;

    lws_cancel_service(m_context);

    m_thread.join();
}

}
}
}
