#include <graphics_api/platform.h>
#include <GL/gl3w.h>


void
platform_setup(roa_renderer_ctx_t ctx)
{
  
}


void
platform_update(roa_renderer_ctx_t ctx)
{

}


void
platform_render(roa_renderer_ctx_t ctx)
{
  unsigned count = roa_array_count(ctx->mesh_renderpasses);
  unsigned i, j;

  for (i = 0; i < count; ++i)
  {
    struct mesh_renderpass *rp = &ctx->mesh_renderpasses[i];

    /* set camera stuff */


    unsigned draw_call = roa_array_count(rp->draw_calls);

    for (j = 0; j < draw_call; ++i)
    {
      /* draw */
    }
  }

  glClearColor(1,1,0,1);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}



void
platform_destroy(roa_renderer_ctx_t ctx)
{

}
