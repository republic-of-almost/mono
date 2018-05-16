#ifdef ROA_RENDERER_API_GL4

#include <graphics_api/platform.h>
#include <roa_lib/array.h>
#include <roa_lib/assert.h>
#include <GL/gl3w.h>
#include <roa_math/math.h>


void
platform_render(roa_renderer_ctx_t ctx)
{
  /* param check */
  ROA_ASSERT(ctx);

  float aspect = (float)ctx->device_settings.device_viewport[0] / (float)ctx->device_settings.device_viewport[1];

  roa_mat4 world;
  roa_mat4_id(&world);

  roa_mat4 proj;
  roa_mat4_projection(&proj, ROA_QUART_TAU * 0.25f, 0.1, 100.f, aspect);

  roa_mat4 view;
  roa_mat4_lookat(&view, roa_float3_set_with_values(4, 4, 4), roa_float3_zero(), roa_float3_set_with_values(0, 1, 0));

  roa_mat4 wvp;
  roa_mat4_multiply_three(&wvp, &world, &view, &proj);

  glBindVertexArray(ctx->graphics_api.vao);

  glViewport(0, 0, ctx->device_settings.device_viewport[0], ctx->device_settings.device_viewport[1]);

  /* fill buffer */
  {
    glPushDebugGroup(GL_DEBUG_SOURCE_APPLICATION, GL_DEBUG_TYPE_PUSH_GROUP, -1, "GBuffer:Fill");

    glUseProgram(ctx->graphics_api.gbuffer_fill.program);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, ctx->graphics_api.gbuffer.fbo);

    glClearColor(1, 0, 1, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

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

    glUniformMatrix4fv(ctx->graphics_api.gbuffer_fill.uni_wvp, 1, GL_FALSE, wvp.data);
    glUniformMatrix4fv(ctx->graphics_api.gbuffer_fill.uni_world, 1, GL_FALSE, world.data);

    glDrawArrays(GL_TRIANGLES, 0, 36);

    glPopDebugGroup();
  }

  /* blit to screen */
  {
    glPushDebugGroup(GL_DEBUG_SOURCE_APPLICATION, GL_DEBUG_TYPE_PUSH_GROUP, -1, "Backbuffer:Blit");

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
    glBindTexture(GL_TEXTURE_2D, ctx->graphics_api.gbuffer.texture_output[3]);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glUniform1i(glGetUniformLocation(ctx->graphics_api.blit.program, "gColorMap"), 0);
    
    glDrawArrays(GL_TRIANGLES, 0, 3);

    glPopDebugGroup();
  }
}

#endif /* ROA_RENDERER_API_GL4 */
