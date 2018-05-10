#include <roa_renderer/roa_renderer.h>
#include <roa_lib/hash.h>
#include <roa_lib/assert.h>
#include <roa_lib/array.h>
#include <roa_lib/spin_lock.h>
#include <string.h>
#include <ctx/ctx.h>


/* ---------------------------------------------------- [ Mesh Resources ] -- */


unsigned
roa_renderer_mesh_resource_count(
  roa_renderer_ctx_t ctx)
{
  return 0;
}


uint64_t
roa_renderer_mesh_resource_add(
	roa_renderer_ctx_t ctx,
	const struct roa_renderer_mesh_resource *rsrc)
{
	/* param check */
	ROA_ASSERT(ctx);
	ROA_ASSERT(rsrc);
	ROA_ASSERT(rsrc->name);

	uint64_t hash_id = roa_hash(rsrc->name);

	/* search for existing */
  {
    roa_spin_lock_aquire(&ctx->mesh_rsrc.lock);

    unsigned i;
    uint64_t *ids = ctx->mesh_rsrc.ids;
    unsigned rsrc_count = roa_array_size(ctx->mesh_rsrc.ids);

    for (i = 0; i < rsrc_count; ++i)
    {
      /* if found we copy the data */
      if (ids[i] == hash_id)
      {
        roa_spin_lock_release(&ctx->mesh_rsrc.lock);
        return hash_id;
      }
    }
  }

  /* add new */
  {

    /* push new mesh */

  }

  roa_spin_lock_release(&ctx->mesh_rsrc.lock);
	return hash_id;
}


int
roa_renderer_mesh_resource_remove(
  roa_renderer_ctx_t ctx,
  const char *name)
{

}
