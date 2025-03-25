#pragma once

#include <gkr/api.hpp>

#ifndef BUILDING_GKR_PIPES
#define GKR_PIPES_API GKR_API_IMPORT
#else
#define GKR_PIPES_API GKR_API_EXPORT
#endif
