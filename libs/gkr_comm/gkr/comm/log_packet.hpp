#pragma once

#include <cinttypes>

namespace gkr
{
namespace comm
{

struct log_packet_head
{
    std::uint16_t packet_size;
    std::uint8_t  packet_ver;
    std::uint8_t  control;
    std::uint32_t pid;
    std:: int64_t stamp;
};
static_assert((sizeof(log_packet_head) % 8) == 0, "Please keep size of log_packet_head multiple of 8");

enum : std::uint8_t
{
    log_packet_version_0_1   = 1,

    log_packet_control_idle  = 0,
    log_packet_control_msg   = 1,

    log_packet_control_start = 2,
    log_packet_control_stop  = 3,

    log_packet_control_thread_spawn = 4,
    log_packet_control_thread_exit  = 5,
};

struct log_packet_msg_data
{
    std:: int64_t tid;
    std:: int16_t severity;
    std:: int16_t facility;
    std::uint32_t line;

    std::uint16_t offset_to_text;
    std::uint16_t offset_to_file;
    std::uint16_t offset_to_func;
    std::uint16_t offset_to_thread;
    std::uint16_t offset_to_channel;
    std::uint16_t offset_to_severity;
    std::uint16_t offset_to_facility;
};
struct log_packet_host_data
{
    std:: int64_t tid;

    std::uint16_t offset_to_thread;
    std::uint16_t offset_to_process;
    std::uint16_t offset_to_host;
};
struct log_packet_thread_data
{
    std:: int64_t tid;

    std::uint16_t offset_to_thread;
};

}
}
