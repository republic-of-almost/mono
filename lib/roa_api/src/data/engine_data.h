#ifndef ENGINE_DATA_INCLUDED_FF1EEF75_58FA_4F83_B99D_D7F7EC949C49
#define ENGINE_DATA_INCLUDED_FF1EEF75_58FA_4F83_B99D_D7F7EC949C49


typedef struct roa_ctx * roa_ctx_t;
typedef struct roa_dispatcher_ctx * roa_dispatcher_ctx_t;


void
rep_data_init();


void
rep_data_destroy();


roa_ctx_t
rep_data_ctx();


roa_dispatcher_ctx_t
rep_data_dispatcher();


#endif /* inc guard */