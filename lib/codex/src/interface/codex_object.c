#include <codex/codex.h>
#include <interface/codex_ctx.h>
#include <roa_lib/fundamental.h>
#include <roa_lib/array.h>
#include <roa_lib/assert.h>
#include <stdlib.h>


/* ------------------------------------------------------ [ Codex Object ] -- */


unsigned
codex_object_create(
  struct codex_ctx *ctx)
{
  /* param check */
  ROA_ASSERT(ctx);

  if(ctx == ROA_NULL)
  {
    return 0;
  }

  /* new obj */
  unsigned new_id = ++ctx->object_instance_counter;
  {
    ROA_ASSERT(new_id != 0);
    roa_array_push(ctx->object_ids, new_id);
    roa_array_push(ctx->parent_ids, 0);
  }

  return new_id;
}


CODEX_BOOL
codex_object_destroy(
  struct codex_ctx *ctx,
  unsigned obj_id)
{
  /* param check */
  ROA_ASSERT(ctx);
  ROA_ASSERT(obj_id);

  if(ctx == ROA_NULL)
  {
    return CODEX_FALSE;
  }

  /* find ID and return true / false */
  {
    unsigned *obj_ids = ctx->object_ids;
    unsigned *parent_ids = ctx->parent_ids;
    const unsigned obj_count = roa_array_size(obj_ids);

    unsigned i;

    for (i = 0; i < obj_count; ++i)
    {
      if (obj_ids[i] == obj_id)
      {
        roa_array_erase(obj_ids, i);
        roa_array_erase(parent_ids, i);

        return CODEX_TRUE;
      }
    }
  }

  return CODEX_FALSE;
}


unsigned
codex_object_count(
  struct codex_ctx *ctx)
{
  /* param check */
  ROA_ASSERT(ctx);

  if(ctx == ROA_NULL)
  {
    return 0;
  }

  return roa_array_size(ctx->object_ids);
}


CODEX_BOOL
codex_object_exists(
  struct codex_ctx            *ctx,
  unsigned                    obj_id)
{
  /* param check */
  ROA_ASSERT(ctx);

  if(ctx == ROA_NULL)
  {
    return CODEX_FALSE;
  }

  /* loop through and look for the id */
  {
    const unsigned *ids = ctx->object_ids;
    const unsigned count = roa_array_size(ids);

    unsigned i;

    for (i = 0; i < count; ++i)
    {
      if (ids[i] == obj_id)
      {
        return CODEX_TRUE;
      }
    }
  }

  return CODEX_FALSE;
}