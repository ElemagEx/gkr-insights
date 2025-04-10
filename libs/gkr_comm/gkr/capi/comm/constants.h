#pragma once

#define COMM_LOG_CHANNEL_NAME                           "GKR-Communication"

#define COMM_PROVIDER_NAME_LIBWEBSOCKET                 "libwebsocket"

#define COMM_SERVICE_NAME_LOG_UPSTREAM_CLIENT           "log-upstream-client"
#define COMM_SERVICE_NAME_LOG_UPSTREAM_SERVER           "log-upstream-server"

#define COMM_TRANSPORT_WEB_SOCKET_SECURE                "web-socket-secure"
#define COMM_TRANSPORT_WEB_SOCKET_PLAIN                 "web-socket-plain"

#define COMM_PORT_LOG_UPSTREAM_WEB_SOCKET_SECURE        9301 /*443*/
#define COMM_PORT_LOG_UPSTREAM_WEB_SOCKET_PLAIN         9302 /*80*/

#define COMM_PARAM_PROTOCOL_SERVICE_PROVIDER            "protocol/service-provider"
#define COMM_PARAM_PROTOCOL_TRANSPORT                   "protocol/transport"
#define COMM_PARAM_PROTOCOL_FORMAT                      "protocol/format"
#define COMM_PARAM_PROTOCOL_VERSION                     "protocol/version"
#define COMM_PARAM_PROTOCOL_REMOTE_SERVER               "protocol/remote-server"
#define COMM_PARAM_PROTOCOL_REMOTE_PORT                 "protocol/remote-port"
#define COMM_PARAM_PROTOCOL_LISTEN_PORT                 "protocol/listen-port"

#define COMM_PARAM_BRIDGE_SEND_QUEUE_INIT_ELEMENT_COUNT "bridge/send-queue-init-count"
#define COMM_PARAM_BRIDGE_SEND_QUEUE_INIT_ELEMENT_SIZE  "bridge/send-queue-init-size"
#define COMM_PARAM_BRIDGE_SEND_QUEUE_RESERVE_FACTOR     "bridge/send-queue-res-factor"

#define COMM_PARAM_BRIDGE_RECV_QUEUE_INIT_ELEMENT_COUNT "bridge/recv-queue-init-count"
#define COMM_PARAM_BRIDGE_RECV_QUEUE_INIT_ELEMENT_SIZE  "bridge/recv-queue-init-size"
#define COMM_PARAM_BRIDGE_RECV_QUEUE_RESERVE_FACTOR     "bridge/recv-queue-res-factor"

#define COMM_PARAM_RETRY_INIT_TIMEOUT_MS                "retry/init-timeout-ms"
#define COMM_PARAM_RETRY_NUM_RETRIES                    "retry/num-retries"
#define COMM_PARAM_RETRY_BACKOFF_MULTIPLIER             "retry/backoff-multiplier"
#define COMM_PARAM_RETRY_JITTER_PERCENT                 "retry/jitter-percent"

#define COMM_PARAM_WEB_SOCKET_PING_TIMEOUT_MS           "web-socket/ping-timeout-ms"
#define COMM_PARAM_WEB_SOCKET_HANGUP_TIMEOUT_MS         "web-socket/hangup-timeout-ms"
