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
  /* VAO */
  {
    GLuint vao = 0;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(&vao);

    if (glObjectLabel) {
      glObjectLabel(GL_VERTEX_ARRAY, vao, -1, "ROARenderer:VAO");
    }

    ctx->graphics_api.vao = vao;
  }

  /* GBuffer */
  {
    GLsizei width = ctx->device_settings.device_viewport[0];
    GLsizei height = ctx->device_settings.device_viewport[1];

    /* FBO */
    {
      GLuint fbo = 0;
      glGenFramebuffers(1, &fbo);
      glBindFramebuffer(GL_FRAMEBUFFER, fbo);

      if (glObjectLabel) {
        glObjectLabel(GL_FRAMEBUFFER, fbo, -1, "GBuffer:FBO");
      }

      ctx->graphics_api.gbuffer.fbo = fbo;
    }

    /* depth texture */
    {
      GLuint depth_texture = 0;
      glGenTextures(1, &depth_texture);
      glBindTexture(GL_TEXTURE_2D, depth_texture);
      glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32F, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, ROA_NULL);

      if (glObjectLabel) {
        glObjectLabel(GL_TEXTURE, depth_texture, -1, "GBuffer:Depth");
      }

      glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depth_texture, 0);

      ctx->graphics_api.gbuffer.texture_depth = depth_texture;
    }

    /* output textures */
    {
      GLuint output_textures[ROA_ARR_COUNT(ctx->graphics_api.gbuffer.texture_output)];
      ROA_MEM_ZERO(output_textures);

      const char *debug_texture_names[] = {
        "GBuffer:WorldPos",
        "GBuffer:Diffuse",
        "GBuffer:Normal",
        "GBuffer:TexCoords",
      };

      glGenTextures(ROA_ARR_COUNT(output_textures), ROA_ARR_DATA(output_textures));

      int count = ROA_ARR_COUNT(output_textures);
      int i;

      for (i = 0; i < count; ++i)
      {
        glBindTexture(GL_TEXTURE_2D, output_textures[i]);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, width, height, 0, GL_RGB, GL_FLOAT, NULL);

        if (glObjectLabel) {
          glObjectLabel(GL_TEXTURE, output_textures[i], -1, debug_texture_names[i]);
        }

        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, output_textures[i], 0);
      }

      GLenum draw_buffers[] = {
        GL_COLOR_ATTACHMENT0,
        GL_COLOR_ATTACHMENT1,
        GL_COLOR_ATTACHMENT2,
        GL_COLOR_ATTACHMENT3,
      };

      glDrawBuffers(ROA_ARR_COUNT(draw_buffers), draw_buffers);

      GLenum fbo_status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
      ROA_ASSERT(fbo_status == GL_FRAMEBUFFER_COMPLETE);

      memcpy(ctx->graphics_api.gbuffer.texture_output, output_textures, sizeof(output_textures));
    }

    /* unbind state */
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
    glBindTexture(GL_TEXTURE_2D, 0);
  }
}


void
platform_destroy(roa_renderer_ctx_t ctx)
{
  ROA_UNUSED(ctx);

  /* param check */
  ROA_ASSERT(ctx);
}

#endif /* ROA_RENDERER_API_GL4 */
