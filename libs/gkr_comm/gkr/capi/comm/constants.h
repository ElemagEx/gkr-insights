#pragma once

#define COMM_TRANSPORT_RAW_UDP                          ""
#define COMM_TRANSPORT_RAW_TCP                          ""
#define COMM_TRANSPORT_WEB_SOCKET                       ""

#define COMM_SERVICE_NAME_LOG_UPSTREAM_WS_CLIENT        "log-upstream-ws-client"
#define COMM_SERVICE_NAME_LOG_UPSTREAM_WS_SERVER        "log-upstream-ws-server"

#define COMM_PORT_WEB_SOCKET_LOG_UPSTREAM               9301

#define COMM_PARAM_LOG_UPSTREAM_SERVICE_PROVIDER        "log-upstream/provider"
#define COMM_PARAM_LOG_UPSTREAM_TRANSPORT               "log-upstream/transport"
#define COMM_PARAM_LOG_UPSTREAM_FORMAT                  "log-upstream/format"
#define COMM_PARAM_LOG_UPSTREAM_VERSION                 "log-upstream/version"

#define COMM_PARAM_BRIDGE_SEND_QUEUE_INIT_ELEMENT_COUNT "bridge/send-queue-init-count"
#define COMM_PARAM_BRIDGE_SEND_QUEUE_INIT_ELEMENT_SIZE  "bridge/send-queue-init-size"
#define COMM_PARAM_BRIDGE_SEND_QUEUE_RESERVE_FACTOR     "bridge/send-queue-res-factor"

#define COMM_PARAM_BRIDGE_RECV_QUEUE_INIT_ELEMENT_COUNT "bridge/recv-queue-init-count"
#define COMM_PARAM_BRIDGE_RECV_QUEUE_INIT_ELEMENT_SIZE  "bridge/recv-queue-init-size"
#define COMM_PARAM_BRIDGE_RECV_QUEUE_RESERVE_FACTOR     "bridge/recv-queue-res-factor"

#define COMM_PARAM_URL_SCHEME                           "url/scheme"
#define COMM_PARAM_URL_HOST                             "url/host"
#define COMM_PARAM_URL_PORT                             "url/port"
#define COMM_PARAM_URL_PATH                             "url/path"

#define COMM_PARAM_RETRY_INIT_TIMEOUT_MS                "retry/init-timeout-ms"
#define COMM_PARAM_RETRY_NUM_RETRIES                    "retry/num-retries"
#define COMM_PARAM_RETRY_BACKOFF_MULTIPLIER             "retry/backoff-multiplier"
#define COMM_PARAM_RETRY_JITTER_PERCENT                 "retry/jitter-percent"

#define COMM_PARAM_WEB_SOCKET_PING_TIMEOUT_MS           "web-socket/ping-timeout-ms"
#define COMM_PARAM_WEB_SOCKET_HANGUP_TIMEOUT_MS         "web-socket/hangup-timeout-ms"
