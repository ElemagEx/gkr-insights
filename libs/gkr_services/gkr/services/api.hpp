#pragma once

#include <gkr/api.hpp>

#ifndef BUILDING_GKR_SERVICES
#define GKR_SERVICES_API GKR_API_IMPORT
#else
#define GKR_SERVICES_API GKR_API_EXPORT
#endif
