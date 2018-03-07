#include <roa_graph/roa_graph.h>
#include <graph_data.h>
#include <roa_lib/assert.h>
#include <roa_lib/alloc.h>
#include <roa_lib/array.h>
#include <roa_lib/spin_lock.h>
#include <roa_lib/atomic.h>


void
roa_graph_ctx_create(roa_graph_ctx_t *ctx)
{
  /* param check */
  ROA_ASSERT(ctx);

  struct roa_graph_ctx *new_ctx = roa_zalloc(sizeof(*new_ctx));
  ROA_ASSERT(new_ctx);

  if (new_ctx)
  {
    roa_spin_lock_init(&new_ctx->objects_lock);

    roa_atomic_int_store(&new_ctx->object_counter, 0);

    roa_array_create(new_ctx->objects, 128);
    roa_array_create(new_ctx->object_ids, 128);

    *ctx = new_ctx;
  }
}


void
roa_graph_ctx_destroy(roa_graph_ctx_t *ctx)
{
  /* param check */
  ROA_ASSERT(ctx);

  struct roa_graph_ctx *die_ctx = *ctx;

  roa_array_destroy(die_ctx->object_ids);
  roa_array_destroy(die_ctx->objects);

  roa_free(die_ctx);
}
