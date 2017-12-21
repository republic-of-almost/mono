#include <codex/codex.h>
#include <interface/codex_ctx.h>
#include <roalib/array.h>
#include <assert.h>


/* ---------------------------------------- [ Codex Object Relationships ] -- */


CODEX_BOOL
codex_object_set_parent(
  struct codex_ctx *ctx,
  unsigned this_id,
  unsigned parent_id)
{
  /* param check */
  assert(ctx);
  assert(this_id);

  /* check not self asignment */
  {
    if (this_id == parent_id)
    {
      return CODEX_FALSE;
    }
  }

  /* validate parent id */
  CODEX_BOOL found_parent = parent_id ? CODEX_FALSE : CODEX_TRUE;

  if(found_parent == CODEX_FALSE)
  {
    const unsigned *ids = ctx->object_ids;
    const unsigned count = roa_array_size(ids);

    for (unsigned i = 0; i < count; ++i)
    {
      if (ids[i] == parent_id)
      {
        found_parent = CODEX_TRUE;
        break;
      }
    }
  }

  if (found_parent == CODEX_FALSE)
  {
    return CODEX_FALSE;
  }

  /* find this and add parent */
  {
    const unsigned *ids = ctx->object_ids;
    unsigned *parent_ids = ctx->parent_ids;
    const unsigned count = roa_array_size(ids);

    for (unsigned i = 0; i < count; ++i)
    {
      if (ids[i] == this_id)
      {
        parent_ids[i] = parent_id;
        return CODEX_TRUE;
      }
    }
  }

  return CODEX_FALSE;
}


void
codex_object_get_children(
  struct codex_ctx *ctx,
  unsigned *obj_id,
  unsigned *count)
{
  /* param check */
  assert(ctx);
  assert(count);
  
  /*  */
}