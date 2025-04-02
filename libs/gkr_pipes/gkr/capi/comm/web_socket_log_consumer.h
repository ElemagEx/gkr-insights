#pragma once

#include <gkr/capi/comm/api.h>

#include <gkr/capi/log/consumer.h>

#ifdef __cplusplus
extern "C" {
#endif

struct gkr_comm_web_socket_log_consumer_callbacks
{
    struct gkr_log_consumer_opt_callbacks opt_callbacks;
};

struct gkr_params;

GKR_CLIENT_COMM_API int gkr_comm_add_web_socket_log_consumer(
    void* channel,
    void* param,
    const struct gkr_comm_web_socket_log_consumer_callbacks* callbacks,
    const struct gkr_params* parameters,
    size_t root
    );
GKR_CLIENT_COMM_API int gkr_comm_add_web_socket_log_consumer_ex(
    void* channel,
    void* param,
    const struct gkr_comm_web_socket_log_consumer_callbacks* callbacks,
    const char* url,
    const char* provider_name,
    const struct gkr_params* parameters,
    size_t root
    );

#ifdef __cplusplus
}
#endif
