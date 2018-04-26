#include <graphics_api/platform.h>
#include <roa_lib/array.h>
#include <GL/gl3w.h>


void
platform_render(roa_renderer_ctx_t ctx)
{
  glClearColor(1,1,0,1);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glBindVertexArray(ctx->gfx_api.vao);

  unsigned count = roa_array_size(ctx->mesh_renderpasses);
  unsigned i, j;

  for (i = 0; i < count; ++i)
  {
    struct mesh_renderpass *rp = &ctx->mesh_renderpasses[i];

    glUseProgram(ctx->gfx_api.mesh_program);
    glBindBuffer(GL_ARRAY_BUFFER, ctx->gfx_api.vbo);

    GLint posAttrib = glGetAttribLocation(ctx->gfx_api.mesh_program, "vs_in_position");
    glEnableVertexAttribArray(posAttrib);
    glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);

    unsigned draw_call = roa_array_size(rp->draw_calls);

    for (j = 0; j < draw_call; ++j)
    {
      GLint uni_wvp = glGetUniformLocation(ctx->gfx_api.mesh_program, "uni_wvp_mat");
      glUniformMatrix4fv(uni_wvp, 1, GL_FALSE, rp->draw_calls[j].world_view_projection);

      glDrawArrays(GL_TRIANGLES, 0, 36);
    }

    roa_array_destroy(rp->draw_calls);
    rp->draw_calls = ROA_NULL;
  }
}
