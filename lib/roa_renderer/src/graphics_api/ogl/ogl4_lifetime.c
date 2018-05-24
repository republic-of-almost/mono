#ifdef ROA_RENDERER_API_GL4


#include <graphics_api/platform.h>
#include <graphics_api/ogl/ogl4_helpers.h>
#include <GL/gl3w.h>
#include <roa_lib/assert.h>
#include <roa_lib/array.h>
#include <scratch/glsl.h>
#include <scratch/geometry.h>
#include <stdlib.h>
#include <string.h>


void
platform_internal_create_gbuffer(roa_renderer_ctx_t ctx)
{
  /* GBuffer */
  {
    GLsizei width  = ctx->device_settings.device_viewport[0];
    GLsizei height = ctx->device_settings.device_viewport[1];

    /* FBO */
    {
      GLuint fbo = 0;
      glGenFramebuffers(1, &fbo);
      glBindFramebuffer(GL_FRAMEBUFFER, fbo);

      if (OGL4_DEBUG_LABLES && glObjectLabel) {
        glObjectLabel(GL_FRAMEBUFFER, fbo, -1, "GBuffer:FBO");
      }

      ctx->graphics_api.gbuffer.fbo = fbo;
    }

    /* depth texture */
    {
      GLuint depth_texture = 0;
      glGenTextures(1, &depth_texture);
      glBindTexture(GL_TEXTURE_2D, depth_texture);
      glTexImage2D(
        GL_TEXTURE_2D,
        0,
        GL_DEPTH_COMPONENT32F,
        width,
        height,
        0,
        GL_DEPTH_COMPONENT,
        GL_FLOAT,
        ROA_NULL);

      if (OGL4_DEBUG_LABLES && glObjectLabel) {
        glObjectLabel(GL_TEXTURE, depth_texture, -1, "GBuffer:Depth");
      }

      glFramebufferTexture2D(
        GL_FRAMEBUFFER,
        GL_DEPTH_ATTACHMENT,
        GL_TEXTURE_2D,
        depth_texture,
        0);
      
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

      ctx->graphics_api.gbuffer.texture_depth = depth_texture;
    }

    /* output textures */
    {
      struct ogl_gbuffer *buffer = &ctx->graphics_api.gbuffer;
    
      GLuint output_textures[ROA_ARR_COUNT(buffer->texture_output)];
      ROA_MEM_ZERO(output_textures);

      const char *debug_texture_names[] = {
        "GBuffer:WorldPos",
        "GBuffer:Diffuse",
        "GBuffer:Normal",
        "GBuffer:TexCoords",
      };

      glGenTextures(
        ROA_ARR_COUNT(output_textures),
        ROA_ARR_DATA(output_textures));

      int count = ROA_ARR_COUNT(output_textures);
      int i;

      for (i = 0; i < count; ++i)
      {
        glBindTexture(GL_TEXTURE_2D, output_textures[i]);
        glTexImage2D(
          GL_TEXTURE_2D,
          0,
          GL_RGBA32F,
          width,
          height,
          0,
          GL_RGBA,
          GL_FLOAT,
          NULL);
        
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        if (OGL4_DEBUG_LABLES && glObjectLabel) {
          glObjectLabel(
            GL_TEXTURE,
            output_textures[i],
            -1,
            debug_texture_names[i]);
        }

        glFramebufferTexture2D(
          GL_FRAMEBUFFER,
          GL_COLOR_ATTACHMENT0 + i,
          GL_TEXTURE_2D,
          output_textures[i],
          0);
      }

      GLenum draw_buffers[] = {
        GL_COLOR_ATTACHMENT0,
        GL_COLOR_ATTACHMENT1,
        GL_COLOR_ATTACHMENT2,
        GL_COLOR_ATTACHMENT3,
      };

      glDrawBuffers(ROA_ARR_COUNT(draw_buffers), draw_buffers);

      GLenum fbo_status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
      GLenum complete   = GL_FRAMEBUFFER_COMPLETE;

      ROA_ASSERT(fbo_status == complete);

      void *dst = (void*)ctx->graphics_api.gbuffer.texture_output;
      memcpy(dst, output_textures, sizeof(output_textures));
    }

    /* unbind state */
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
    glBindTexture(GL_TEXTURE_2D, 0);
  }
}


void
platform_internal_destroy_gbuffer(roa_renderer_ctx_t ctx)
{
  (void)ctx;
  /* todo :) */
}


void
platform_setup(roa_renderer_ctx_t ctx)
{
  /* clear arr */
  {
    while(glGetError());
  }

  /* VAO */
  {
    GLuint vao = 0;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    if (OGL4_DEBUG_LABLES && glObjectLabel) {
      glObjectLabel(GL_VERTEX_ARRAY, vao, -1, "ROARenderer:VAO");
    }

    ctx->graphics_api.vao = vao;
  }

  glrPushMarkerGroup("Setup");

  platform_internal_create_gbuffer(ctx);

  /* general init */
  {
    roa_array_create_with_capacity(ctx->graphics_api.mesh_ids, 32);
    roa_array_create_with_capacity(ctx->graphics_api.meshes, 32);
  }

  /* err */
  {
    GLuint err = glGetError();
    if (err)
    {
      ROA_ASSERT(0);
    }
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
          " DiffuseOut      = texture(gColorMap, TexCoord0).xyz;\n"
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

        if (OGL4_DEBUG_LABLES && glObjectLabel) {
          glObjectLabel(GL_PROGRAM, prog, -1, "GBuffer:Fill");
        }

        glDeleteShader(vert_shd);
        glDeleteShader(frag_shd);

        ctx->graphics_api.gbuffer_fill.program = prog;
      }
    }

    /* uniforms */
    {
      struct ogl_gbuffer_fill_pass *fill = &ctx->graphics_api.gbuffer_fill;
      GLuint program = fill->program;
    
      fill->uni_wvp     = glGetUniformLocation(program, "gWVP");
      fill->uni_world   = glGetUniformLocation(program, "gWorld");
      fill->uni_diffuse = glGetUniformLocation(program, "gColorMap");
    }
    
    /* input */
    {
      GLuint program = ctx->graphics_api.gbuffer_fill.program;
      
      GLint pos = glGetAttribLocation(program, "Position");
      
      ctx->graphics_api.gbuffer_fill.input[0].loc = pos;
      ctx->graphics_api.gbuffer_fill.input[0].normalize = GL_FALSE;
      ctx->graphics_api.gbuffer_fill.input[0].size = 8 * sizeof(GLfloat);
      ctx->graphics_api.gbuffer_fill.input[0].component_count = 3;
      ctx->graphics_api.gbuffer_fill.input[0].ptr = 0;
      
      GLint texc = glGetAttribLocation(program, "TexCoord");
      
      ctx->graphics_api.gbuffer_fill.input[1].loc = texc;
      ctx->graphics_api.gbuffer_fill.input[1].normalize = GL_FALSE;
      ctx->graphics_api.gbuffer_fill.input[1].size = 8 * sizeof(GLfloat);
      ctx->graphics_api.gbuffer_fill.input[1].component_count = 2;
      ctx->graphics_api.gbuffer_fill.input[1].ptr = (void*)(3 * sizeof(GLfloat));

      GLint norm = glGetAttribLocation(program, "Normal");

      ctx->graphics_api.gbuffer_fill.input[2].loc = norm;
      ctx->graphics_api.gbuffer_fill.input[2].normalize = GL_TRUE;
      ctx->graphics_api.gbuffer_fill.input[2].size = 8 * sizeof(GLfloat);
      ctx->graphics_api.gbuffer_fill.input[2].component_count = 3;
      ctx->graphics_api.gbuffer_fill.input[2].ptr = (void*)(5 * sizeof(GLfloat));
    }
  }

  /* err */
  {
    GLuint err = glGetError();
    if (err)
    {
      ROA_ASSERT(0);
    }
  }

  /* decal */
  {
    /* program */
    {
      GLuint vert_shd = glCreateShader(GL_VERTEX_SHADER);
      {
        const char *vs = ""
          "#version 410\n"
          "\n"
          "uniform mat4 gView;\n"
          "uniform mat4 gProjection;\n"
          "\n"
          "uniform mat4 modelMatrix;\n"
          "uniform vec3 decalSize;\n"
          "\n"
          "layout ( location = 0 ) in vec4 positionIN;\n"
          "layout ( location = 1 ) in vec2 uvIN;\n"
          "layout ( location = 2 ) in vec3 normalIN;\n"
          "\n"
          "out vec4 posFS;\n"
          "out vec4 posW;\n"
          "out vec2 uvFS;\n"
          "\n"
          "void main()\n"
          "{\n"
            "posW = modelMatrix * vec4(positionIN.xyz * 1, positionIN.w);\n"
            "//Move position to clip space\n"
            "posFS = gProjection * gView * posW;\n"
            "uvFS = uvIN;\n"
            "gl_Position = posFS;\n"
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
        const char *fs = ""
          "#version 410\n"
          //"#extension GL_ARB_texture_rectangle : enable\n"
          "\n"
          "in vec4 posFS;\n"
          "in vec4 posW;\n"
          "in vec2 uvFS;\n"
          "\n"
          "uniform mat4 modelMatrix;\n"
          "uniform sampler2D gDiffuse;\n"
          "uniform sampler2D gNormalDepth;\n"
          "uniform sampler2D gWorldPos;\n"
          "\n"
          "uniform float gGamma;\n"
          "\n"
          "uniform mat4 invProjView;\n"
          "uniform mat4 invModelMatrix;\n"
          "\n"
          "vec4 reconstruct_pos(float z, vec2 uv_f)\n"
          "{\n"
          "vec4 sPos = vec4(uv_f * 2.0 - 1.0, z, 1.0);\n"
          "sPos = invProjView * sPos;\n"
          "return vec4((sPos.xyz / sPos.w ), sPos.w);\n"
          "}\n"
          "\n"
          "\n"
          "layout ( location = 1 ) out vec4 diffuseRT;\n"
          "layout ( location = 2 ) out vec4 specularRT;\n"
          "layout ( location = 3 ) out vec4 glowMatIDRT;\n"
          "\n"
          "void main()\n"
          "{\n"
          "vec2 screenPosition = posFS.xy / posFS.w;\n"
          "\n"
          "vec2 depthUV = screenPosition * 0.5f + 0.5f;\n"
          //"depthUV += vec2(0.5f / 1280.0f, 0.5f / 720.0f); //half pixel offset\n"
          "float depth = texture(gNormalDepth, depthUV).r;\n"
          "\n"
          "vec4 worldPos = texture(gWorldPos, depthUV);\n"
          //"vec4 worldPos = reconstruct_pos(depth, depthUV);\n"
          "worldPos.w = 1;"
          "vec4 localPos = worldPos * invModelMatrix;\n"
          "\n"
          "float dist = 0.5f - abs(localPos.y);\n"
          "float dist2 = 0.5f - abs(localPos.x);\n"
          "float dist3 = 0.5f - abs(localPos.z);\n"
          "\n"
          "if ((depth < 1.0 && dist > 0 && dist2 > 0 && dist3 > 0))\n"
          "{\n"
          "vec2 uv = vec2(localPos.x, localPos.y) + 0.5f;\n"
          "vec4 diffuseColor = texture(gDiffuse, uv);\n"
          "diffuseRT = diffuseColor;\n"
          "diffuseRT = vec4(0.0, 1.0, 0.0, 1);\n"
          "}\n"
          "else {\n"
          "discard; }\n"
          //"diffuseRT = worldPos;\n"
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
        
        if (OGL4_DEBUG_LABLES && glObjectLabel) {
          glObjectLabel(GL_PROGRAM, prog, -1, "Decal");
        }

        glDeleteShader(vert_shd);
        glDeleteShader(frag_shd);

        ctx->graphics_api.decal.program = prog; 
      }
    }
    
    /* input */
    {
      struct ogl_decal *decal = &ctx->graphics_api.decal;
      GLuint program = decal->program;
      
      GLint pos = glGetAttribLocation(program, "positionIN");
      
      decal->input[0].loc             = pos;
      decal->input[0].normalize       = GL_FALSE;
      decal->input[0].size            = 9 * sizeof(GLfloat);
      decal->input[0].component_count = 4;
      decal->input[0].ptr             = 0;
      
      GLint texc = glGetAttribLocation(program, "uvIN");
      
      decal->input[1].loc             = texc;
      decal->input[1].normalize       = GL_FALSE;
      decal->input[1].size            = 9 * sizeof(GLfloat);
      decal->input[1].component_count = 2;
      decal->input[1].ptr             = (void*)(4 * sizeof(GLfloat));

      GLint norm = glGetAttribLocation(program, "normalIN");

      decal->input[2].loc             = norm;
      decal->input[2].normalize       = GL_TRUE;
      decal->input[2].size            = 9 * sizeof(GLfloat);
      decal->input[2].component_count = 3;
      decal->input[2].ptr             = (void*)(6 * sizeof(GLfloat));
    }

    /* volume */
    {
      float data[1024];

      geom_vert_desc vert_desc[] = {
        GEOM_VERT_POSITION4,
        GEOM_UV,
        GEOM_NORMAL,
      };

      unsigned vert_count = 0;

      geometry_generate_cube(
        ROA_ARR_DATA(vert_desc),
        ROA_ARR_COUNT(vert_desc),
        0.5f,
        0.5f,
        0.5f,
        ROA_ARR_DATA(data),
        &vert_count);
    
      GLuint vbo;
      glGenBuffers(1, &vbo);
      glBindBuffer(GL_ARRAY_BUFFER, vbo);

      if (OGL4_DEBUG_LABLES && glObjectLabel) {
        glObjectLabel(GL_BUFFER, vbo, -1, "Decal:Volume(P3N3T2)");
      }

      glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vert_count, data, GL_STATIC_DRAW);

      ctx->graphics_api.decal.vbo = vbo;
    }
  }

  /* err */
  {
    GLuint err = glGetError();
    if (err)
    {
      ROA_ASSERT(0);
    }
  }

  /* fullscreen */
  {
    /* program */
    {
      GLuint vert_shd = glCreateShader(GL_VERTEX_SHADER);
      {
        const char vs[] = ""
          "#version 330\n"

          "layout(location = 0) in vec2 Position;\n"
          "layout(location = 1) in vec2 TexCoord; \n"

          "out vec2 TexCoord0;\n"

          "void main()\n"
          "{\n"
          " gl_Position = vec4(Position, 0.0, 1.0);\n"
          " TexCoord0 = TexCoord;\n"
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

          "uniform sampler2D gColorMap;\n"

          "layout (location = 0) out vec4 final_color;\n"

          "void main()  \n"
          "{\n"
          " final_color      = texture(gColorMap, TexCoord0);\n"
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

        if (OGL4_DEBUG_LABLES && glObjectLabel) {
          glObjectLabel(GL_PROGRAM, prog, -1, "FullscreenBlit");
        }

        glDeleteShader(vert_shd);
        glDeleteShader(frag_shd);

        ctx->graphics_api.blit.program = prog;
      }
    }

    /* triangle */
    {
      float verts[] = {
        /* x y, s t */
        -1.f, +3.f, 0.f, 2.f,
        -1.f, -1.f, 0.f, 0.f,
        +3.f, -1.f, 2.f, 0.f,
      };

      GLuint vbo;
      glGenBuffers(1, &vbo);
      glBindBuffer(GL_ARRAY_BUFFER, vbo);
      glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);
    
      if (OGL4_DEBUG_LABLES && glObjectLabel) {
        glObjectLabel(GL_BUFFER, vbo, -1, "FullscreenTriangle(f2:f2)");
      }

      ctx->graphics_api.blit.fullscreen_triangle = vbo;
    }
  }

  /* err */
  {
    GLuint err = glGetError();
    if (err)
    {
      ROA_ASSERT(0);
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

    if (OGL4_DEBUG_LABLES && glObjectLabel) {
      glObjectLabel(GL_BUFFER, vbo, -1, "Temp:Cube");
    }

    ctx->graphics_api.vbo = vbo;
  }

  glrPopMarkerGroup();
}


void
platform_resize(roa_renderer_ctx_t ctx)
{
  /* delete fbo */
  platform_internal_destroy_gbuffer(ctx);

  /* recreate fbo */
  platform_internal_create_gbuffer(ctx);
}


void
platform_destroy(roa_renderer_ctx_t ctx)
{
  ROA_UNUSED(ctx);

  /* param check */
  ROA_ASSERT(ctx);
}


#endif /* ROA_RENDERER_API_GL4 */
