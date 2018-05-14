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
  //gl3wInit();

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

  glPushDebugGroup(GL_DEBUG_SOURCE_APPLICATION, GL_DEBUG_TYPE_PUSH_GROUP, -1, "Setup");

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

  /* gbuffer fill */
  {
    /* program */
    {
      GLuint vert_shd = glCreateShader(GL_VERTEX_SHADER);
      {
        const char vs[] = ""
          "#version 330\n"

          "layout(location = 0) in vec3 Position;\n"
          "layout(location = 1) in vec2 TexCoord; \n"
          "layout(location = 2) in vec3 Normal;\n"

          "uniform mat4 gWVP;\n"
          "uniform mat4 gWorld;\n"

          "out vec2 TexCoord0;\n"
          "out vec3 Normal0;\n"
          "out vec3 WorldPos0;\n"

          "void main()\n"
          "{\n"
          " gl_Position = gWVP * vec4(Position, 1.0);\n"
          " TexCoord0 = TexCoord;\n"
          " Normal0 = (gWorld * vec4(Normal, 0.0)).xyz;\n"
          " WorldPos0 = (gWorld * vec4(Position, 1.0)).xyz;\n"
          "}\n";

        const GLchar *src = vs;
        glShaderSource(vert_shd, 1, &src, ROA_NULL);
        glCompileShader(vert_shd);

        GLint status;
        glGetShaderiv(vert_shd, GL_COMPILE_STATUS, &status);

        if (status != GL_TRUE)
        {
          char buffer[512];
          glGetShaderInfoLog(vert_shd, 512, NULL, buffer);
          ROA_ASSERT(ROA_FALSE);
        }
      }

      GLuint frag_shd = glCreateShader(GL_FRAGMENT_SHADER);
      {
        const char fs[] = ""
          "#version 330\n"

          "in vec2 TexCoord0;   \n"
          "in vec3 Normal0;   \n"
          "in vec3 WorldPos0;  \n"

          "uniform sampler2D gColorMap;\n"

          "layout (location = 0) out vec3 WorldPosOut;\n"
          "layout (location = 1) out vec3 DiffuseOut;\n"
          "layout (location = 2) out vec3 NormalOut;\n"
          "layout (location = 3) out vec3 TexCoordOut;\n"

          "void main()  \n"
          "{\n"
          " WorldPosOut     = WorldPos0;\n"
          " DiffuseOut      = vec3(1, 0.3, 0); /*texture(gColorMap, TexCoord0).xyz;*/\n"
          " NormalOut       = normalize(Normal0);\n"
          " TexCoordOut     = vec3(TexCoord0, 0.0);\n"
          "}\n";

        const GLchar *src = fs;
        glShaderSource(frag_shd, 1, &src, ROA_NULL);
        glCompileShader(frag_shd);

        GLint status;
        glGetShaderiv(frag_shd, GL_COMPILE_STATUS, &status);

        if (status != GL_TRUE)
        {
          char buffer[512];
          glGetShaderInfoLog(frag_shd, 512, NULL, buffer);
          ROA_ASSERT(ROA_FALSE);
        }
      }

      {
        GLuint prog = glCreateProgram();
        glAttachShader(prog, vert_shd);
        glAttachShader(prog, frag_shd);
        glLinkProgram(prog);

        GLint status;
        glGetProgramiv(prog, GL_LINK_STATUS, &status);

        if (status != GL_TRUE)
        {
          char buffer[512];
          glGetProgramInfoLog(prog, 512, NULL, buffer);
          ROA_ASSERT(ROA_FALSE);
        }

        if (glObjectLabel) {
          glObjectLabel(GL_PROGRAM, prog, -1, "GBuffer:Fill");
        }

        glDeleteShader(vert_shd);
        glDeleteShader(frag_shd);

        ctx->graphics_api.gbuffer_fill.program = prog;
      }
    }

    /* uniforms */
    {
      ctx->graphics_api.gbuffer_fill.uni_wvp = glGetUniformLocation(ctx->graphics_api.gbuffer_fill.program, "gWVP");
      ctx->graphics_api.gbuffer_fill.uni_world = glGetUniformLocation(ctx->graphics_api.gbuffer_fill.program, "gWorld");
    }
  }

  /* temp */
  {
    geom_vert_desc vert_desc[] = {
      GEOM_VERT_POSITION3,
      GEOM_UV,
      GEOM_NORMAL,
    };

    float geom[2048];
    unsigned count = 0;

    geometry_generate_cube(
      ROA_ARR_DATA(vert_desc),
      ROA_ARR_COUNT(vert_desc),
      1,
      1,
      1,
      ROA_ARR_DATA(geom),
      &count);

    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * count, geom, GL_STATIC_DRAW);

    if (glObjectLabel) {
      glObjectLabel(GL_BUFFER, vbo, -1, "Temp:Cube");
    }

    ctx->graphics_api.vbo = vbo;
  }

  glPopDebugGroup();
}


void
platform_destroy(roa_renderer_ctx_t ctx)
{
  ROA_UNUSED(ctx);

  /* param check */
  ROA_ASSERT(ctx);
}

#endif /* ROA_RENDERER_API_GL4 */
