#include <codex/codex.h>
#include <interface/codex_ctx.h>
#include <roalib/array.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>


/* ---------------------------------------------------- [ Codex Lifetime ] -- */


void
codex_create(
  struct codex_ctx **ctx)
{
  /* param check */
  assert(ctx);

  const unsigned codex_bytes = sizeof(struct codex_ctx);
  struct codex_ctx *new_ctx = (struct codex_ctx*)malloc(codex_bytes);
  assert(new_ctx);

  if (new_ctx)
  {
    memset(new_ctx, 0, codex_bytes);

    const unsigned obj_size_hint = 256;
    roa_array_create(new_ctx->object_ids, obj_size_hint);
    roa_array_create(new_ctx->parent_ids, obj_size_hint);

    const unsigned callback_size_hint = 16;
    roa_array_create(new_ctx->callbacks_created, callback_size_hint);
    roa_array_create(new_ctx->callbacks_destroyed, callback_size_hint);

    *ctx = new_ctx;
  }
}


void
codex_destroy(
  struct codex_ctx **ctx)
{
  /* param check */
  assert(ctx);

  if(*ctx)
  {
    roa_array_destroy((*ctx)->object_ids);
    roa_array_destroy((*ctx)->parent_ids);

    roa_array_destroy((*ctx)->callbacks_created);
    roa_array_destroy((*ctx)->callbacks_destroyed);

    free(*ctx);
  }
}