#ifndef GRAPH_INCLUDED_1F6E4996_5978_493F_B059_2DAAFA571A81
#define GRAPH_INCLUDED_1F6E4996_5978_493F_B059_2DAAFA571A81


#include <roa_lib/fundamental.h>
#include <roa_math/math.h> /* do I want this to be floats instead ? */


#ifdef __cplusplus
extern "C" {
#endif


typedef struct roa_graph_ctx * roa_graph_ctx_t;


/* --------------------------------------------------------- [ Lifetime ] -- */


void
roa_graph_ctx_create(
  roa_graph_ctx_t *                 ctx);


void
roa_graph_ctx_destroy(
  roa_graph_ctx_t *                 ctx);


/* ------------------------------------------------------------ [ Nodes ] -- */


ROA_BOOL
roa_graph_node_exists(
  const roa_graph_ctx_t             data,
  const uint32_t                    node_id,
  unsigned *                        out_index); /* optional */


uint32_t
roa_graph_node_create(
  roa_graph_ctx_t                   data);


ROA_BOOL
roa_graph_node_remove(
  roa_graph_ctx_t                   data,
  const uint32_t                    node_id);


ROA_BOOL
roa_graph_node_set_parent(
  roa_graph_ctx_t                   data,
  const uint32_t                    child_id,
  const uint32_t                    parent_id); /* zero if un-parenting */


unsigned
roa_graph_node_child_count(
  const roa_graph_ctx_t             data,
  const uint32_t                    node_id);


unsigned
roa_graph_node_descendants_count(
  const roa_graph_ctx_t             data,
  const uint32_t                    node_id);


uint32_t
roa_graph_node_get_child(
  const roa_graph_ctx_t             data,
  const uint32_t                    node_id,
  const unsigned                    child_index);


uint32_t
roa_graph_node_get_parent(
  const roa_graph_ctx_t             data,
  const uint32_t                    node_id);


 /* ------------------------------------------------------ [ Attributes ] -- */


ROA_BOOL
roa_graph_node_get_name(
  const roa_graph_ctx_t graph,
  const uint32_t node_id,
  const char **name);


ROA_BOOL
roa_graph_node_set_name(
  roa_graph_ctx_t graph,
  const uint32_t node_id,
  const char *name);


ROA_BOOL
roa_graph_node_get_tags(
  const roa_graph_ctx_t graph,
  const uint32_t node_id,
  uint64_t *tags);


ROA_BOOL
roa_graph_node_set_tags(
  roa_graph_ctx_t graph,
  const uint32_t node_id,
  const uint64_t tags);


ROA_BOOL
roa_graph_node_get_transform(
  const roa_graph_ctx_t data,
  const uint32_t node_id,
  roa_transform *trans,
  const ROA_BOOL inherited);


ROA_BOOL
roa_graph_node_set_transform(
  roa_graph_ctx_t data,
  const uint32_t node_id,
  const roa_transform *trans);


ROA_BOOL
roa_graph_node_register_type(
  roa_graph_ctx_t data,
  const uint32_t node_id,
  const uint64_t type_id
);


ROA_BOOL
roa_graph_node_unregister_type(
  roa_graph_ctx_t data,
  const uint32_t node_id,
  const uint64_t type_id
);


ROA_BOOL
roa_graph_node_get_data_type_id(
  const roa_graph_ctx_t data,
  const uint32_t node_id,
  uint64_t *type_id);


ROA_BOOL
roa_graph_node_get_user_data(
  const roa_graph_ctx_t data,
  const uint32_t node_id,
  void **user_data);


ROA_BOOL
roa_graph_node_set_user_data(
  roa_graph_ctx_t data,
  const uint32_t node_id,
  const void *user_data);


#ifdef __cplusplus
} /* extern */
#endif

#endif /* inc guard */