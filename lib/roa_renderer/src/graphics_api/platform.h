#ifndef PLATFORM_INCLUDED_
#define PLATFORM_INCLUDED_


#include <ctx/ctx.h>


void
platform_setup(roa_renderer_ctx_t ctx);


void
platform_update(roa_renderer_ctx_t ctx);


void
platform_render(roa_renderer_ctx_t ctx);


void
platform_destroy(roa_renderer_ctx_t ctx);


#endif /* inc guard */
