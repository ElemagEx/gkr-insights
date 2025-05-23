#pragma once

#include <gkr/comm/api.hpp>

#include <gkr/comm/end_point.hpp>
#include <gkr/comm/bridge.hpp>

#include <gkr/concurency/worker_thread.hpp>
#include <gkr/concurency/waitable_event.hpp>

#include <vector>
#include <memory>

namespace gkr
{
class params;

namespace comm
{

class log_collector;

class log_receiver
    : public worker_thread
    , public end_point
{
    log_receiver           (const log_receiver&) noexcept = delete;
    log_receiver& operator=(const log_receiver&) noexcept = delete;

    log_receiver           (log_receiver&&) noexcept = delete;
    log_receiver& operator=(log_receiver&&) noexcept = delete;

public:
    GKR_COMM_API log_receiver();
    GKR_COMM_API virtual ~log_receiver() noexcept(DIAG_NOEXCEPT) override;

public:
    GKR_COMM_API bool configure(
        int port,
        const char* transport,
        const char* provider_name = nullptr,
        const params* parameters = nullptr,
        std::size_t root = 0
        );
    GKR_COMM_API bool configure(
        const params& parameters,
        std::size_t root = 0
        );

public:
    bool is_successfully_configured() const noexcept
    {
        return bool(m_bridge);
    }

protected:
    virtual const char* get_name() noexcept override;

    virtual long long get_wait_timeout_ns() noexcept override;

    virtual std::size_t get_waitable_objects_count() noexcept override;

    virtual waitable_object& get_waitable_object(std::size_t index) noexcept override;

    virtual bool on_start() override;
    virtual void on_finish() override;

    virtual void on_cross_action(action_id_t action, void* param, void* result) override;
    virtual void on_queue_action(action_id_t action, void* data) override;

    virtual void on_wait_timeout() override;
    virtual void on_wait_success(std::size_t index) override;

    virtual bool on_exception(except_method_t method, const std::exception* e) noexcept override;

protected:
    virtual void on_error(int error, const void* data, std::size_t size) override;
    virtual void on_connect(connection* conn) override;
    virtual void on_disconnect(connection* conn) override;
    virtual void on_data_received(connection* conn) override;

public:
    bool add_collector(std::shared_ptr<log_collector> collector);
    bool del_collector(std::shared_ptr<log_collector> collector);

private:
    waitable_event m_data_received_event;

    std::vector<std::shared_ptr<log_collector>> m_collectors;

    std::shared_ptr<bridge> m_bridge;
};

}
}
