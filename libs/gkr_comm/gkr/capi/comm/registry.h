#include <gkr/capi/comm/api.h>

#ifdef __cplusplus
extern "C" {
#endif

struct gkr_comm_provider;

GKR_COMM_API int gkr_comm_providers_registry_init(int clients_only);
GKR_COMM_API int gkr_comm_providers_registry_done();

GKR_COMM_API int gkr_comm_providers_count_in_registry();

GKR_COMM_API struct gkr_comm_provider* gkr_comm_providers_get_from_registry(int index);
GKR_COMM_API struct gkr_comm_provider* gkr_comm_providers_find_in_registry(const char* name);

GKR_COMM_API int gkr_comm_providers_start_all();
GKR_COMM_API int gkr_comm_providers_stop_all();

GKR_COMM_API const char* gkr_comm_provider_get_name(struct gkr_comm_provider* provider);

GKR_COMM_API struct gkr_comm_provider* gkr_comm_client_register_provider(const char* name);
GKR_COMM_API struct gkr_comm_provider* gkr_comm_server_register_provider(const char* name);

#ifdef __cplusplus
}
#endif
