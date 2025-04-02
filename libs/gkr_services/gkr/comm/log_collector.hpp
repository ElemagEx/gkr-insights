#pragma once

namespace gkr
{
namespace comm
{

struct log_packet_head;

class log_collector
{
public:
    GKR_SERVER_COMM_API virtual ~log_collector();

    virtual bool init_collecting() = 0;
    virtual void done_collecting() = 0;

    virtual bool  filter_log_packet(const log_packet_head& packet) = 0;
    virtual void consume_log_packet(const log_packet_head& packet) = 0;
};

}
}
