#include <codex/codex.h>
#include <interface/codex_ctx.h>
#include <roa_lib/array.h>
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

  if(ctx && this_id)
  {
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

      unsigned i;

      for (i = 0; i < count; ++i)
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

      unsigned i;

      for (i = 0; i < count; ++i)
      {
        if (ids[i] == this_id)
        {
          parent_ids[i] = parent_id;
          return CODEX_TRUE;
        }
      }
    }
  }

  return CODEX_FALSE;
}


void
codex_object_get_children(
  struct codex_ctx *ctx,
  unsigned obj_id,
  unsigned *out_child_ids,
  unsigned *out_count)
{
  /* param check */
  assert(ctx);
  assert(out_count);

  /* find all children */
  if(ctx && out_count)
  {
    *out_count = 0;

    const unsigned *ids = ctx->object_ids;
    const unsigned *parent_ids = ctx->parent_ids;
    const unsigned count = roa_array_size(ctx->parent_ids);

    unsigned i;

    for (i = 0; i < count; ++i)
    {
      if (parent_ids[i] == obj_id)
      {
        if (out_child_ids)
        {
          const unsigned index = *out_count;
          out_child_ids[index] = ids[i];
        }

        ++(*out_count);
      }
    }
  }
}


void
codex_object_get_parent(
  struct codex_ctx *ctx,
  unsigned obj_id,
  unsigned *out_parent_id)
{
  /* param check */
  assert(ctx);
  assert(obj_id);
  assert(out_parent_id);

  if (ctx && obj_id && out_parent_id)
  {
    *out_parent_id = 0;

    /* find this index */
    {
      const unsigned *obj_ids    = ctx->object_ids;
      const unsigned *parent_ids = ctx->parent_ids;
      const unsigned obj_count   = roa_array_size(obj_ids);

      unsigned i;

      for (i = 0; i < obj_count; ++i)
      {
        if (obj_ids[i] == obj_id)
        {
          *out_parent_id = parent_ids[i];
          break;
        }
      }
    }
  }
}