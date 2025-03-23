#pragma once

#ifdef BUILDING_GKR_MEDIATOR
#   ifdef _WIN32
#       define GKR_MEDIATOR_API __declspec(dllexport)
#   else
#       define GKR_MEDIATOR_API __attribute__((__visibility__("default")))
#   endif
#else
#   ifdef _WIN32
#       define GKR_MEDIATOR_API __declspec(dllimport)
#   else
#       define GKR_MEDIATOR_API
#   endif
#endif
