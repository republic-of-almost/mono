#include <codex/codex.h>
#include <interface/codex_ctx.h>
#include <roa_lib/array.h>
#include <assert.h>
#include <stdlib.h>


/* ------------------------------------------------------ [ Codex Object ] -- */


void
codex_object_callbacks(
  struct codex_ctx *ctx,
  struct codex_callbacks *callbacks)
{
  /* param check */
  assert(ctx);
  assert(callbacks);

  if (callbacks->created)
  {
    struct codex_callback cb;
    cb.function = (void*)callbacks->created;
    cb.user_data = callbacks->created_user_data;

    roa_array_push(ctx->callbacks_created, cb);
  }

  if (callbacks->destroyed)
  {
    struct codex_callback cb;
    cb.function = (void*)callbacks->destroyed;
    cb.user_data = callbacks->destroyed_user_data;

    roa_array_push(ctx->callbacks_destroyed, cb);
  }
}


unsigned
codex_object_create(
  struct codex_ctx *ctx)
{
  /* param check */
  assert(ctx);

  /* new obj */
  unsigned new_id = ++ctx->object_instance_counter;
  {
    assert(new_id != 0);
    roa_array_push(ctx->object_ids, new_id);
    roa_array_push(ctx->parent_ids, 0);
  }

  /* alert callbacks */
  {
    struct codex_callback *callbacks = ctx->callbacks_created;
    const unsigned count = roa_array_size(callbacks);

    unsigned i;

    for (i = 0; i < count; ++i)
    {
      codex_object_created_callback_fn func = (codex_object_created_callback_fn)callbacks[i].function;
      void *user_data = callbacks[i].user_data;

      func(&new_id, 1, user_data);
    }
  }

  return new_id;
}


CODEX_BOOL
codex_object_destroy(
  struct codex_ctx *ctx,
  unsigned obj_id)
{
  /* param check */
  assert(ctx);
  assert(obj_id);

  /* find any children and remove parent */
  {
    /* todo - build a list of ids to remove */
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

        /* callbacks */
        {
          struct codex_callback *callbacks = ctx->callbacks_destroyed;
          const unsigned cb_count = roa_array_size(callbacks);

          unsigned j;

          for (j = 0; j < cb_count; ++j)
          {
            codex_object_created_callback_fn func = (codex_object_created_callback_fn)callbacks[j].function;
            void *user_data = callbacks[j].user_data;

            func(&obj_id, 1, user_data);
          }
        }

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
  assert(ctx);

  return roa_array_size(ctx->object_ids);
}


CODEX_BOOL
codex_object_exists(
  struct codex_ctx            *ctx,
  unsigned                    obj_id)
{
  /* param check */
  assert(ctx);

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