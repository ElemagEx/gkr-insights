#pragma once

#define COMM_SERVICE_NAME_LOG_CONSUMER      "gkr-log-consumer"

#define COMM_PORT_WEB_SOCKET_LOG_UPSTREAM   9301

#define COMM_PARAM_BRIDGE_SEND_QUEUE_INIT_ELEMENT_COUNT "bridge/send-queue-count"
#define COMM_PARAM_BRIDGE_SEND_QUEUE_INIT_ELEMENT_SIZE  "bridge/send-queue-size"
#define COMM_PARAM_BRIDGE_SEND_QUEUE_RESERVE_PERCENTAGE "bridge/send-queue-incr"

#define COMM_PARAM_BRIDGE_RECV_QUEUE_INIT_ELEMENT_COUNT "bridge/recv-queue-count"
#define COMM_PARAM_BRIDGE_RECV_QUEUE_INIT_ELEMENT_SIZE  "bridge/recv-queue-size"
#define COMM_PARAM_BRIDGE_RECV_QUEUE_RESERVE_PERCENTAGE "bridge/recv-queue-incr"

#define COMM_PARAM_URL_SCHEME                           "url/scheme"
#define COMM_PARAM_URL_HOST                             "url/host"
#define COMM_PARAM_URL_PORT                             "url/port"
#define COMM_PARAM_URL_PATH                             "url/path"

#define COMM_PARAM_RETRY_INIT_TIMEOUT_MS                "retry/init-timeout-ms"
#define COMM_PARAM_RETRY_NUM_RETRIES                    "retry/num-retries"
#define COMM_PARAM_RETRY_BACKOFF_MULTIPLIER             "retry/backoff-multiplier"
#define COMM_PARAM_RETRY_JITTER_PERCENT                 "retry/jitter-percent"

#define COMM_PARAM_WEB_SOCKET_PROVIDER                  "web-socket/provider"
#define COMM_PARAM_WEB_SOCKET_PING_TIMEOUT_MS           "web-socket/ping-timeout-ms"
#define COMM_PARAM_WEB_SOCKET_HANGUP_TIMEOUT_MS         "web-socket/hangup-timeout-ms"
