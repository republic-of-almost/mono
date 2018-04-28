#ifndef CONFIG_INCLUDED_939DF286_FC86_448D_8D74_5EB3C6E1F4C2
#define CONFIG_INCLUDED_939DF286_FC86_448D_8D74_5EB3C6E1F4C2


#include <roa_lib/fundamental.h>


void
rep_config_init();


uint64_t
rep_config_tagged_hash_logic();


uint64_t
rep_config_tagged_hash_rendering();


uint64_t
rep_config_tagged_hash_physics();


#define REP_DATA_TYPEID_CAMERA ROA_BIT(1)
#define REP_DATA_TYPEID_RENDERER_MESH ROA_BIT(2)


#endif /* inc guard */