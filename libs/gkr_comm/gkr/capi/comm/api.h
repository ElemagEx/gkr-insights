#pragma once

#include <gkr/capi/api.h>

#ifndef BUILDING_GKR_COMM
#define GKR_COMM_API            GKR_API_IMPORT
#else
#define GKR_COMM_API            GKR_API_EXPORT
#endif
#ifndef BUILDING_GKR_PIPES
#define GKR_COMM_PIPES_API      GKR_API_IMPORT
#else
#define GKR_COMM_PIPES_API      GKR_API_EXPORT
#endif
#ifndef BUILDING_GKR_SERVICES
#define GKR_COMM_SERVICES_API   GKR_API_IMPORT
#else
#define GKR_COMM_SERVICES_API   GKR_API_EXPORT
#endif
