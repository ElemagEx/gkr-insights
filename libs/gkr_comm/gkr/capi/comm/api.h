#pragma once

#include <gkr/capi/api.h>

#ifndef BUILDING_GKR_COMM
#define GKR_COMM_API            GKR_API_IMPORT
#else
#define GKR_COMM_API            GKR_API_EXPORT
#endif
#ifndef BUILDING_GKR_CLIENT_COMM
#define GKR_CLIENT_COMM_API     GKR_API_IMPORT
#else
#define GKR_CLIENT_COMM_API     GKR_API_EXPORT
#endif
#ifndef BUILDING_GKR_SERVER_COMM
#define GKR_SERVER_COMM_API     GKR_API_IMPORT
#else
#define GKR_SERVER_COMM_API   GKR_API_EXPORT
#endif
