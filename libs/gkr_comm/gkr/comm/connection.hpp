#pragma once

#include <gkr/comm/api.hpp>

#include <cstddef>

namespace gkr
{
namespace comm
{

class connection
{
    unsigned m_id;

protected:
    GKR_COMM_API connection();
    GKR_COMM_API virtual ~connection();

public:
    unsigned id() const
    {
        return m_id;
    }
public:
    virtual const char* addr() const = 0;
    virtual const char* path() const = 0;
};

}
}
