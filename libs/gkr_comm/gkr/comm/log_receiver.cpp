#include <gkr/defs.hpp>

#include <gkr/comm/log_receiver.hpp>

namespace gkr
{
namespace comm
{

log_receiver::log_receiver()
{
    check_args_order();
}

log_receiver::~log_receiver() noexcept(DIAG_NOEXCEPT)
{
    join(true);
}

const char* log_receiver::get_name() noexcept
{
    return "log-receiver";
}

long long log_receiver::get_wait_timeout_ns() noexcept
{
    return WAIT_INFINITE;
}

std::size_t log_receiver::get_waitable_objects_count() noexcept
{
    return 1;
}

waitable_object& log_receiver::get_waitable_object(std::size_t index) noexcept
{
    Check_ValidState(index == 0);
    return m_data_received_event;
}

bool log_receiver::on_start()
{
    return true;
}

void log_receiver::on_finish()
{
}

void log_receiver::on_cross_action(action_id_t action, void* param, void* result)
{
}

void log_receiver::on_queue_action(action_id_t action, void* data)
{
}

void log_receiver::on_wait_timeout()
{
    Check_Failure();
}

void log_receiver::on_wait_success(std::size_t index)
{
}

bool log_receiver::on_exception(except_method_t method, const std::exception* e) noexcept
{
    return true;
}

bool log_receiver::on_error(int evt, void* data, std::size_t size)
{
    return false;
}

void log_receiver::on_connect()
{
}

void log_receiver::on_disconnect()
{
}

void log_receiver::on_data_received()
{
}

bool log_receiver::add_collector(std::shared_ptr<log_collector> collector)
{
    return false;
}

bool log_receiver::del_collector(std::shared_ptr<log_collector> collector)
{
    return false;
}

}
}
