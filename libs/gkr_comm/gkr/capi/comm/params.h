#pragma once

#define PARAM_BRIDGE_QUEUE_INCOMING_ELEMENTS_INIT_COUNT "bridge/i-queue-count"
#define PARAM_BRIDGE_QUEUE_INCOMING_ELEMENTS_INIT_SIZE  "bridge/i-queue-size"
#define PARAM_BRIDGE_QUEUE_INCOMING_RESERVE_PERCENTAGE  "bridge/i-queue-incr"

#define PARAM_BRIDGE_QUEUE_OUTGOING_ELEMENTS_INIT_COUNT "bridge/o-queue-count"
#define PARAM_BRIDGE_QUEUE_OUTGOING_ELEMENTS_INIT_SIZE  "bridge/o-queue-size"
#define PARAM_BRIDGE_QUEUE_OUTGOING_RESERVE_PERCENTAGE  "bridge/o-queue-incr"

#define PARAM_URL_SCHEME                                "url/scheme"
#define PARAM_URL_HOST                                  "url/host"
#define PARAM_URL_PORT                                  "url/port"
#define PARAM_URL_PATH                                  "url/path"

#define PARAM_RETRY_INIT_TIMEOUT_MS                     "retry/init-timeout-ms"
#define PARAM_RETRY_NUM_RETRIES                         "retry/num-retries"
#define PARAM_RETRY_BACKOFF_MULTIPLIER                  "retry/backoff-multiplier"
#define PARAM_RETRY_JITTER_PERCENT                      "retry/jitter-percent"

#define PARAM_WEB_SOCKET_PING_TIMEOUT_MS                "web-socket/ping-timeout-ms"
#define PARAM_WEB_SOCKET_HANGUP_TIMEOUT_MS              "web-socket/hangup-timeout-ms"
