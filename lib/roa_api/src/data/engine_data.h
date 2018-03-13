#ifndef ENGINE_DATA_INCLUDED_FF1EEF75_58FA_4F83_B99D_D7F7EC949C49
#define ENGINE_DATA_INCLUDED_FF1EEF75_58FA_4F83_B99D_D7F7EC949C49


typedef struct roa_ctx * roa_ctx_t;
typedef struct roa_dispatcher_ctx * roa_dispatcher_ctx_t;
typedef struct volt_ctx * volt_ctx_t;
typedef struct roa_graph_ctx * roa_graph_ctx_t;

void
rep_data_init();


void
rep_data_destroy();


roa_ctx_t
rep_data_ctx();


roa_dispatcher_ctx_t
rep_data_dispatcher();


struct roa_tagged_allocator*
rep_data_dispatcher_allocator();


volt_ctx_t
rep_data_volt();


roa_graph_ctx_t
rep_data_graph();


#endif /* inc guard */
