#ifndef ENGINE_DATA_INCLUDED_FF1EEF75_58FA_4F83_B99D_D7F7EC949C49
#define ENGINE_DATA_INCLUDED_FF1EEF75_58FA_4F83_B99D_D7F7EC949C49


typedef struct roa_ctx * roa_ctx_t;
typedef struct roa_job_dispatcher_ctx * roa_job_dispatcher_ctx_t;
typedef struct roa_graph_ctx * roa_graph_ctx_t;
typedef struct roa_renderer_ctx * roa_renderer_ctx_t;

struct rep_input_desc;

void
rep_data_init();


void
rep_data_destroy();


roa_ctx_t
rep_data_ctx();


struct rep_input_desc*
rep_data_input_data();


roa_job_dispatcher_ctx_t
rep_data_dispatcher();


struct roa_tagged_allocator*
rep_data_dispatcher_allocator();


roa_graph_ctx_t
rep_data_graph();


roa_renderer_ctx_t
rep_data_renderer();


#endif /* inc guard */
