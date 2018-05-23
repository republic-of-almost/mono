#ifdef ROA_RENDERER_API_GL4

#include <graphics_api/platform.h>
#include <graphics_api/ogl/ogl4_helpers.h>
#include <roa_lib/array.h>
#include <roa_lib/assert.h>
#include <GL/gl3w.h>
#include <roa_math/math.h>


void
platform_render(roa_renderer_ctx_t ctx)
{
  /* param check */
  ROA_ASSERT(ctx);

  /* renderpasses */
  int rp_count = roa_array_size(ctx->renderpass.rps);
  int i;

  glBindVertexArray(ctx->graphics_api.vao);

  GLsizei vp_width = ctx->device_settings.device_viewport[0];
  GLsizei vp_height = ctx->device_settings.device_viewport[1];
  glViewport(0, 0, vp_width, vp_height);

  for (i = 0; i < rp_count; ++i)
  {
    struct renderpass *rp = &ctx->renderpass.rps[i];
    unsigned dc_count = roa_array_size(rp->draw_calls);

    /* fill buffer */
    {
      char buffer[128];
      memset(buffer, 0, sizeof(buffer));
      sprintf(buffer, "GBuffer:Fill - Cam %d - DCs %d", i, dc_count);
      
      glrPushMarkerGroup(buffer);

      glUseProgram(ctx->graphics_api.gbuffer_fill.program);

      glEnable(GL_DEPTH_TEST);
      glEnable(GL_CULL_FACE);

      glBindFramebuffer(GL_DRAW_FRAMEBUFFER, ctx->graphics_api.gbuffer.fbo);

      glClearColor(1, 0, 1, 1);
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

      GLenum draw_buffers[] = {
        GL_COLOR_ATTACHMENT0,
        GL_COLOR_ATTACHMENT1,
        GL_COLOR_ATTACHMENT2,
        GL_COLOR_ATTACHMENT3,
      };

      glDrawBuffers(ROA_ARR_COUNT(draw_buffers), draw_buffers);

      /* draw calls */
      int j;

      for (j = 0; j < dc_count; ++j)
      {
        struct renderpass_draw_call dc = rp->draw_calls[j];

        glBindBuffer(GL_ARRAY_BUFFER, ctx->graphics_api.meshes[0].vbo);

        GLint pos = glGetAttribLocation(ctx->graphics_api.gbuffer_fill.program, "Position");
        glEnableVertexAttribArray(pos);
        glVertexAttribPointer(pos, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), 0);

        GLint texc = glGetAttribLocation(ctx->graphics_api.gbuffer_fill.program, "TexCoord");
        glEnableVertexAttribArray(texc);
        glVertexAttribPointer(texc, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));

        GLint norm = glGetAttribLocation(ctx->graphics_api.gbuffer_fill.program, "Normal");
        glEnableVertexAttribArray(norm);
        glVertexAttribPointer(norm, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(5 * sizeof(GLfloat)));

        GLint texAttrib = glGetAttribLocation(ctx->graphics_api.gbuffer_fill.program, "gColorMap");
        glEnableVertexAttribArray(texAttrib);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, ctx->graphics_api.tex);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glUniformMatrix4fv(ctx->graphics_api.gbuffer_fill.uni_wvp, 1, GL_FALSE, dc.wvp);
        glUniformMatrix4fv(ctx->graphics_api.gbuffer_fill.uni_world, 1, GL_FALSE, dc.world);

        glDrawArrays(GL_TRIANGLES, 0, 36);
      }

      roa_array_destroy(rp->draw_calls);

      glrPopMarkerGroup();
    }

    
    /* decals */
    {
      GLenum draw_buffers[] = {
        GL_COLOR_ATTACHMENT0,
        GL_COLOR_ATTACHMENT1,
        GL_COLOR_ATTACHMENT2,
        GL_COLOR_ATTACHMENT3,
      };

      glDrawBuffers(ROA_ARR_COUNT(draw_buffers), draw_buffers);

      glrPushMarkerGroup("Decals");

      glUseProgram(ctx->graphics_api.decal.program);

      glBindBuffer(GL_ARRAY_BUFFER, ctx->graphics_api.decal.vbo);

      GLint pos = glGetAttribLocation(ctx->graphics_api.gbuffer_fill.program, "positionIN");
      glEnableVertexAttribArray(pos);
      glVertexAttribPointer(pos, 4, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), 0);

      GLint texc = glGetAttribLocation(ctx->graphics_api.gbuffer_fill.program, "uvIN");
      glEnableVertexAttribArray(texc);
      glVertexAttribPointer(texc, 2, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), (void*)(4 * sizeof(GLfloat)));

      GLint norm = glGetAttribLocation(ctx->graphics_api.gbuffer_fill.program, "normalIN");
      glEnableVertexAttribArray(norm);
      glVertexAttribPointer(norm, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), (void*)(6 * sizeof(GLfloat)));

      GLint texAttrib = glGetAttribLocation(ctx->graphics_api.gbuffer_fill.program, "gColorMap");
      glEnableVertexAttribArray(texAttrib);
      glActiveTexture(GL_TEXTURE0);
      glBindTexture(GL_TEXTURE_2D, ctx->graphics_api.tex);

      roa_mat4 id, world, scale, position, inv_world;

      roa_mat4_id(&id);
      roa_mat4_translate(&position, roa_float3_set_with_values(0, 0, 0));
      roa_mat4_scale(&scale, roa_float3_set_with_values(7, 7, 7));
      roa_mat4_multiply_three(&world, &id, &scale, &position);
      roa_mat4_inverse(&inv_world, &world);

      roa_mat4 view_proj, inv_view_proj, view, proj_view, proj;
      roa_mat4_import(&view_proj, rp->camera.view_projection);
      
      roa_mat4_import(&proj, rp->camera.projection);
      roa_mat4_import(&view, rp->camera.view);
      roa_mat4_multiply(&proj_view, &proj, &view);

      roa_mat4_inverse(&inv_view_proj, &proj_view);

      roa_mat4 wvp;
      roa_mat4_multiply(&wvp, &world, &view_proj);
      
      GLint view_uni = glGetUniformLocation(ctx->graphics_api.decal.program, "gView");
      glUniformMatrix4fv(view_uni, 1, GL_FALSE, rp->camera.view);

      GLint proj_uni = glGetUniformLocation(ctx->graphics_api.decal.program, "gProjection");
      glUniformMatrix4fv(proj_uni, 1, GL_FALSE, rp->camera.projection);

      GLint world_uni = glGetUniformLocation(ctx->graphics_api.decal.program, "modelMatrix");
      glUniformMatrix4fv(world_uni, 1, GL_FALSE, world.data);

      GLint inv_view_proj_uni = glGetUniformLocation(ctx->graphics_api.decal.program, "invProjView");
      glUniformMatrix4fv(inv_view_proj_uni, 1, GL_FALSE, inv_view_proj.data);

      GLint inv_world_uni = glGetUniformLocation(ctx->graphics_api.decal.program, "invModelMatrix");
      glUniformMatrix4fv(inv_world_uni, 1, GL_FALSE, inv_world.data);

      glDrawArrays(GL_TRIANGLES, 0, 36);

      glrPopMarkerGroup();
    }
  } /* rps */

  /* blit to screen */
  {
    glrPushMarkerGroup("Backbuffer:Blit");

    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

    glUseProgram(ctx->graphics_api.blit.program);
    glClearColor(1, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glBindBuffer(GL_ARRAY_BUFFER, ctx->graphics_api.blit.fullscreen_triangle);

    GLint pos = glGetAttribLocation(ctx->graphics_api.gbuffer_fill.program, "Position");
    glEnableVertexAttribArray(pos);
    glVertexAttribPointer(pos, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);

    GLint texc = glGetAttribLocation(ctx->graphics_api.gbuffer_fill.program, "TexCoord");
    glEnableVertexAttribArray(texc);
    glVertexAttribPointer(texc, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (void*)(2 * sizeof(GLfloat)));

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, ctx->graphics_api.gbuffer.texture_output[1]);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glUniform1i(glGetUniformLocation(ctx->graphics_api.blit.program, "gColorMap"), 0);
    
    glDrawArrays(GL_TRIANGLES, 0, 3);

    glrPopMarkerGroup();
  }
}

#endif /* ROA_RENDERER_API_GL4 */
