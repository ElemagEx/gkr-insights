#pragma once

#include <gkr/comm/connection.hpp>

#include <string>

struct lws;

namespace gkr
{
namespace comm
{
namespace providers
{
namespace libwebsocket
{

class instance : public connection
{
    struct lws* m_wsi;

public:
    instance(struct lws* wsi);
    virtual ~instance();

protected:
    virtual const char* addr() const override;
    virtual const char* path() const override;

public:
    void request_write();

    bool write_data(const void* data, std::size_t size);

private:
    std::string m_addr;
    std::string m_path;
};

}
}
}
}
