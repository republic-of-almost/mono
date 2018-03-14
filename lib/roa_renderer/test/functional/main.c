#include <roa_renderer/roa_renderer.h>


int
main()
{
	roa_renderer_ctx_t renderer_ctx = ROA_NULL;

	roa_renderer_ctx_create(&renderer_ctx);


	roa_renderer_ctx_destroy(&renderer_ctx);


	return 0;
}
