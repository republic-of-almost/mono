#include <roa_renderer/roa_renderer.h>
#include <ctx/ctx.h>
#include <roa_lib/alloc.h>
#include <roa_lib/assert.h>


/* ---------------------------------------------------------- [ Lifetime ] -- */


ROA_BOOL
roa_renderer_ctx_create(
	roa_renderer_ctx_t *ctx)
{
	/* param check */
	ROA_ASSERT(*ctx);

	struct roa_renderer_ctx *new_ctx = roa_zalloc(sizeof(*new_ctx));
	ROA_ASSERT(new_ctx);

	*ctx = new_ctx;

	return new_ctx ? ROA_TRUE : ROA_FALSE;
}


void
roa_renderer_ctx_execute(
	roa_renderer_ctx_t ctx)
{
	/* param check */
	ROA_ASSERT(ctx);
}


void
roa_renderer_ctx_destroy(
	roa_renderer_ctx_t *ctx)
{
	/* param check */
	ROA_ASSERT(ctx);
	ROA_ASSERT(*ctx);
	
	struct roa_renderer_ctx *kill_ctx = *ctx;
	roa_free(kill_ctx);

	*ctx = ROA_NULL;
}


