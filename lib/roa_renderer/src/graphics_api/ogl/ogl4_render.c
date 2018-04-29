#include <graphics_api/platform.h>
#include <roa_lib/array.h>
#include <GL/gl3w.h>


void
platform_render(roa_renderer_ctx_t ctx)
{
  glClearColor(1,1,0,1);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);

  GLsizei vp_width  = ctx->settings.device_viewport[0];
  GLsizei vp_height = ctx->settings.device_viewport[1];

  glViewport(0, 0, vp_width, vp_height);

  glBindVertexArray(ctx->gfx_api.vao);

  unsigned count = roa_array_size(ctx->mesh_renderpasses);
  unsigned i, j;

  for (i = 0; i < count; ++i)
  {
    struct mesh_renderpass *rp = &ctx->mesh_renderpasses[i];

    glUseProgram(ctx->gfx_api.mesh_program);

    GLint posAttrib = glGetAttribLocation(ctx->gfx_api.mesh_program, "vs_in_position");
    glEnableVertexAttribArray(posAttrib);
    glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), 0);

    glBindBuffer(GL_ARRAY_BUFFER, ctx->gfx_api.vbo);

    unsigned draw_call = roa_array_size(rp->draw_calls);

    for (j = 0; j < draw_call; ++j)
    {
      float *world_mat = rp->draw_calls[j].world;

      GLint uni_world = glGetUniformLocation(ctx->gfx_api.mesh_program, "uni_world_mat");
      glUniformMatrix4fv(uni_world, 1, GL_FALSE, world_mat);

      float *wvp_mat = rp->draw_calls[j].world_view_projection;

      GLint uni_wvp = glGetUniformLocation(ctx->gfx_api.mesh_program, "uni_wvp_mat");
      glUniformMatrix4fv(uni_wvp, 1, GL_FALSE, wvp_mat);

      float color[] = {0.8,0.8,0.85,1};

      GLint u_color = glGetUniformLocation(ctx->gfx_api.mesh_program, "uni_color");
      glUniform4fv(u_color, 1, color);

      glDrawArrays(GL_TRIANGLES, 0, 36);
    }

    roa_array_destroy(rp->draw_calls);
    rp->draw_calls = ROA_NULL;
  }
}
