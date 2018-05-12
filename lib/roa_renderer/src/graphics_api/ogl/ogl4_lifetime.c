#ifdef ROA_RENDERER_API_GL4

#include <graphics_api/platform.h>
#include <GL/gl3w.h>
#include <roa_lib/assert.h>
#include <scratch/glsl.h>
#include <scratch/geometry.h>
#include <stdlib.h>


void
platform_setup(roa_renderer_ctx_t ctx)
{
 
}


void
platform_destroy(roa_renderer_ctx_t ctx)
{
  ROA_UNUSED(ctx);

  /* param check */
  ROA_ASSERT(ctx);
}

#endif /* ROA_RENDERER_API_GL4 */
