#pragma once

#include <gkr/capi/api.h>

#ifndef BUILDING_GKR_COMM
#define GKR_COMM_API            GKR_API_IMPORT
#else
#define GKR_COMM_API            GKR_API_EXPORT
#endif
