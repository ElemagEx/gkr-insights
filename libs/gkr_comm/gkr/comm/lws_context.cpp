#include <gkr/defs.hpp>

#include <gkr/comm/lws_context.hpp>
#include <gkr/comm/lws_protocol.hpp>

#include <gkr/diagnostics.hpp>
#include <gkr/log/defs/generic_cdefs.hpp>

#include <libwebsockets.h>

#include <cstring>
#include <atomic>

namespace libWebSocket
{

Context::Context() noexcept
{
}
Context::~Context() noexcept
{
}

int Context::thread_init()
{
    struct lws_context_creation_info info;
    std::memset(&info, 0, sizeof(info));

    unsigned protocols = 0;
    get_context_info(protocols, info.options);

    unsigned http_dummy_protocol_index = 0;
    get_server_info(info.port, http_dummy_protocol_index, info.mounts);

    m_protocols = static_cast<struct lws_protocols*>(std::malloc(sizeof(struct lws_protocols) * (protocols + 1)));

    Check_NotNullPtr(m_protocols, -1);

    std::memset(m_protocols, 0, sizeof(struct lws_protocols) * (protocols + 1));

    if(info.mounts != nullptr)
    {
        Check_ValidState(http_dummy_protocol_index < protocols, -1);

        m_protocols[http_dummy_protocol_index] = { "http", lws_callback_http_dummy, 0, 0, 0, nullptr, 0 };
    }
    for(unsigned index = 0; index < protocols; ++index)
    {
        if((info.mounts != nullptr) && (index == http_dummy_protocol_index)) continue;

        struct lws_protocols& protocol = m_protocols[index];

        protocol.user = create_protocol(index);

        Check_NotNullPtr(protocol.user, -1);

        protocol.callback = main_protocol_callback;
        protocol.name     = static_cast<Protocol*>(protocol.user)->get_info(
            protocol.id,
            protocol.per_session_data_size,
            protocol.rx_buffer_size,
            protocol.tx_packet_size);
    }
    m_protocols[protocols] = LWS_PROTOCOL_LIST_TERM;

    info.user = this;

    m_context = lws_create_context(&info);

    return (m_context != nullptr) ? +1 : -1;
}

void Context::thread_loop()
{
	for(m_running = true; m_running; )
    {
		if(0 > lws_service(m_context, 0))
        {
            m_running = false;
        }
    }
}

void Context::thread_done()
{
    if(m_protocols != nullptr)
    {
        for(unsigned index = 0; ; ++index)
        {
            struct lws_protocols& protocol = *(m_protocols + index);

            if(protocol.name == nullptr) break;
            if(protocol.user == nullptr) continue;

            delete static_cast<Protocol*>(protocol.user);
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

bool Context::run()
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

bool Context::stop()
{
    Check_ValidState(m_thread.joinable(), false);

    m_running = false;

    lws_cancel_service(m_context);

    m_thread.join();

    return true;
}

}
