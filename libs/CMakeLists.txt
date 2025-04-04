#
# libwebsockets
#
find_package(libwebsockets CONFIG REQUIRED)
list(APPEND CMAKE_MODULE_PATH ${LWS_CMAKE_DIR})
set(requirements 1)
require_lws_config(LWS_ROLE_WS 1 requirements)
require_lws_config(LWS_WITH_SERVER 1 requirements)

add_subdirectory(gkr_comm)
