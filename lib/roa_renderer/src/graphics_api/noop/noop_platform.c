#ifdef ROA_RENDERER_API_NOOP


#include <graphics_api/platform.h>


void platform_setup(roa_renderer_ctx_t ctx)   { ROA_UNUSED(ctx); }
void platform_update(roa_renderer_ctx_t ctx)  { ROA_UNUSED(ctx); }
void platform_render(roa_renderer_ctx_t ctx)  { ROA_UNUSED(ctx); }
void platform_destroy(roa_renderer_ctx_t ctx) { ROA_UNUSED(ctx); }


#endif
