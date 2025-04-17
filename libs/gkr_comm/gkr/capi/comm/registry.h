#include <gkr/capi/comm/api.h>

#ifdef __cplusplus
extern "C" {
#endif

struct gkr_comm_provider;
struct gkr_params;

GKR_COMM_API int gkr_comm_providers_registry_init(int add_log_comm_channel);
GKR_COMM_API int gkr_comm_providers_registry_done();

GKR_COMM_API int gkr_comm_providers_count_in_registry();

GKR_COMM_API struct gkr_comm_provider* gkr_comm_providers_get_from_registry(int index);
GKR_COMM_API struct gkr_comm_provider* gkr_comm_providers_find_in_registry(const char* name);

GKR_COMM_API int gkr_comm_providers_start_all();
GKR_COMM_API int gkr_comm_providers_stop_all();

GKR_COMM_API const char* gkr_comm_provider_get_name(struct gkr_comm_provider* provider);

GKR_COMM_API struct gkr_comm_provider* gkr_comm_register_provider(const char* name);

GKR_COMM_API struct gkr_comm_provider* gkr_comm_register_provider_ex(const char* name, const struct gkr_params* parameters, size_t root);

#ifdef __cplusplus
}
#endif
