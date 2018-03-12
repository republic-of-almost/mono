#include <roa_graph/roa_graph.h>
#include <graph_data.h>
#include <roa_lib/assert.h>
#include <roa_lib/alloc.h>
#include <roa_lib/array.h>
#include <roa_lib/spin_lock.h>
#include <roa_lib/atomic.h>
#include <roa_lib/log.h>


void
roa_graph_ctx_create(roa_graph_ctx_t *ctx)
{
  /* param check */
  ROA_ASSERT(ctx);

  struct roa_graph_ctx *new_ctx = roa_zalloc(sizeof(*new_ctx));
  ROA_ASSERT(new_ctx);

  if (new_ctx)
  {
    unsigned init_capacity = 32;

    roa_array_create_with_capacity(new_ctx->node_id, init_capacity);
    roa_array_create_with_capacity(new_ctx->local_transform, init_capacity);
    roa_array_create_with_capacity(new_ctx->world_transform, init_capacity);
    roa_array_create_with_capacity(new_ctx->parent_depth_data, init_capacity);
    roa_array_create_with_capacity(new_ctx->data, init_capacity);

    new_ctx->instance_counter = 0;

    ROA_LOG_INFO("ROA Graph initialized with inital capacity of %d", init_capacity);

    *ctx = new_ctx;
  }
}


void
roa_graph_ctx_destroy(roa_graph_ctx_t *ctx)
{
  /* param check */
  ROA_ASSERT(ctx);

  struct roa_graph_ctx *die_ctx = *ctx;

  if (die_ctx)
  {
    roa_array_destroy(die_ctx->node_id);
    roa_array_destroy(die_ctx->local_transform);
    roa_array_destroy(die_ctx->world_transform);
    roa_array_destroy(die_ctx->parent_depth_data);
    roa_array_destroy(die_ctx->data);

    ROA_LOG_INFO("ROA Graph destroyed");

    roa_free(die_ctx);
  }
}
