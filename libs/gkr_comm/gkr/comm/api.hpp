#pragma once

#include <gkr/api.hpp>

#ifndef BUILDING_GKR_COMM_X
#define GKR_COMM_API GKR_API_IMPORT
#else
#define GKR_COMM_API GKR_API_EXPORT
#endif
