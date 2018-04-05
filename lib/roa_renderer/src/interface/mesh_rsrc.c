#include <roa_renderer/roa_renderer.h>
#include <roa_lib/hash.h>
#include <roa_lib/assert.h>
#include <ctx/ctx.h>


/* ---------------------------------------------------- [ Mesh Resources ] -- */


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

	/* search for hash */

	return 0;
}
