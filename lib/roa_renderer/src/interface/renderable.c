#include <roa_renderer/roa_renderer.h>
#include <ctx/ctx.h>
#include <roa_lib/assert.h>
#include <roa_lib/array.h>


/* -------------------------------------------------------- [ Renderable ] -- */


ROA_BOOL
roa_renderer_mesh_renderable_set(
  roa_renderer_ctx_t ctx,
  struct roa_renderer_mesh_renderable *renderable,
  uint32_t renderable_id)
{
	/* param check */
	ROA_ASSERT(ctx);
	ROA_ASSERT(renderable);
	ROA_ASSERT(renderable_id);

  /* find key */
  unsigned rdr_count = roa_array_size(ctx->renderable_id);
  unsigned i;

  for (i = 0; i < rdr_count; ++i)
  {
    if (ctx->renderable_id[i] == renderable_id)
    {
      ctx->renderable[i] = *renderable;

      return ROA_TRUE;
    }
  }

  roa_array_push(ctx->renderable_id, renderable_id);
  roa_array_push(ctx->renderable, *renderable);

  return ROA_TRUE;
}


ROA_BOOL
roa_renderer_mesh_renderable_get(
  roa_renderer_ctx_t ctx,
  struct roa_renderer_mesh_renderable *out_renderable,
  uint32_t renderable_id)
{
  /* param check */
  ROA_ASSERT(ctx);
  ROA_ASSERT(out_renderable);
  ROA_ASSERT(renderable_id);

  /* find key */
  unsigned rdr_count = roa_array_size(ctx->renderable_id);
  unsigned i;

  for(i = 0; i < rdr_count; ++i)
  {
    if (ctx->renderable_id[i] == renderable_id)
    {
      *out_renderable = ctx->renderable[i];

      return ROA_TRUE;
    }
  }

  return ROA_FALSE;
}


ROA_BOOL
roa_renderer_renderable_clear(
  roa_renderer_ctx_t ctx,
  uint32_t renderable_id)
{
  /* unused */
  ROA_UNUSED(ctx);
  ROA_UNUSED(renderable_id);

  return ROA_FALSE;
}



ROA_BOOL
roa_renderer_mesh_renderable_clear(
  roa_renderer_ctx_t ctx,
  uint32_t renderable_id)
{
  /* param check */
  ROA_ASSERT(ctx);
  ROA_ASSERT(renderable_id);

  /* find key */
  unsigned rdr_count = roa_array_size(ctx->renderable_id);
  unsigned i;

  for (i = 0; i < rdr_count; ++i)
  {
    if (ctx->renderable_id[i] == renderable_id)
    {
      roa_array_erase(ctx->renderable_id, i);
      roa_array_erase(ctx->renderable, i);

      return ROA_TRUE;
    }
  }

  return ROA_FALSE;
}


unsigned
roa_renderer_mesh_renderable_count(
  roa_renderer_ctx_t ctx)
{
  /* param check */
  ROA_ASSERT(ctx);

  unsigned count = roa_array_size(ctx->renderable_id);

  return count;
}


ROA_BOOL
roa_renderer_mesh_renderable_exists(
  roa_renderer_ctx_t ctx,
  uint32_t renderable_id)
{
  /* param check */
  ROA_ASSERT(ctx);
  ROA_ASSERT(renderable_id);

  /* find key */
  unsigned rdr_count = roa_array_size(ctx->renderable_id);
  unsigned i;

  for (i = 0; i < rdr_count; ++i)
  {
    if (ctx->renderable_id[i] == renderable_id)
    {
      return ROA_TRUE;
    }
  }

  return ROA_FALSE;
}
