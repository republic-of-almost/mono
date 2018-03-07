#ifndef GRAPH_INCLUDED_1F6E4996_5978_493F_B059_2DAAFA571A81
#define GRAPH_INCLUDED_1F6E4996_5978_493F_B059_2DAAFA571A81


#include <roa_lib/fundamental.h>


#ifdef __cplusplus
extern "C" {
#endif


typedef struct roa_graph_ctx * roa_graph_ctx_t;


/* ---------------------------------------------------------- [ Lifetime ] -- */


void
roa_graph_ctx_create(roa_graph_ctx_t *ctx);


void
roa_graph_ctx_destroy(roa_graph_ctx_t *ctx);


/* ------------------------------------------------------------- [ Nodes ] -- */


uint32_t
roa_graph_node_create(roa_graph_ctx_t ctx);


ROA_BOOL
roa_graph_node_set_parent(roa_graph_ctx_t ctx, uint32_t node, uint32_t parent_node);


uint32_t
roa_graph_node_get_parent(roa_graph_ctx_t ctx, uint32_t node);


ROA_BOOL
roa_graph_node_remove(roa_graph_ctx_t ctx, uint32_t node_id);


unsigned
roa_graph_node_count(roa_graph_ctx_t ctx);


#ifdef __cplusplus
} /* extern */
#endif

#endif /* inc guard */