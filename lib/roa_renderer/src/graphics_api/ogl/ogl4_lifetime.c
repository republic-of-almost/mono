#ifdef ROA_RENDERER_API_GL4


#include <graphics_api/platform.h>
#include <graphics_api/ogl/ogl4_helpers.h>
#include <GL/gl3w.h>
#include <roa_lib/assert.h>
#include <roa_lib/array.h>
#include <roa_lib/dir.h>
#include <roa_lib/file.h>
#include <scratch/glsl.h>
#include <scratch/geometry.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>


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

      glrObjectLabel(GL_FRAMEBUFFER, fbo, "GBuffer:FBO");

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

      glrObjectLabel(GL_TEXTURE, depth_texture, "GBuffer:Depth");

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

        glrObjectLabel(GL_TEXTURE, output_textures[i], debug_texture_names[i]);

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

  if (OGL4_ERROR_CHECKS)
  {
    GLuint err = glGetError();
    if (err) {
      ROA_ASSERT(0);
    }
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

    glrObjectLabel(GL_VERTEX_ARRAY, vao, "ROARenderer:VAO");
    

    ctx->graphics_api.vao = vao;
  }

  if (OGL4_ERROR_CHECKS)
  {
    GLuint err = glGetError();
    if (err) {
      ROA_ASSERT(0);
    }
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
        char file[1024];
        memset(file, 0, sizeof(file));

        strcat(file, roa_exe_dir());
        strcat(file, "assets/shaders/");
        strcat(file, "gbuffer_fill.vert");

        const char *vs = 0;
        roa_copy_file(file, (char**)&vs, ROA_NULL);
   
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
        char file[1024];
        memset(file, 0, sizeof(file));

        strcat(file, roa_exe_dir());
        strcat(file, "assets/shaders/");
        strcat(file, "gbuffer_fill.frag");

        const char *fs = 0;
        roa_copy_file(file, (char**)&fs, ROA_NULL);

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

        glrObjectLabel(GL_PROGRAM, prog, "GBuffer:Fill");

        glDeleteShader(vert_shd);
        glDeleteShader(frag_shd);

        ctx->graphics_api.gbuffer_fill.program = prog;
      }
    }

    /* uniforms */
    {
      struct ogl_gbuffer_fill_pass *fill = &ctx->graphics_api.gbuffer_fill;
      GLuint program = fill->program;
    
      fill->uni_wvp     = glGetUniformLocation(program, "uni_wvp");
      fill->uni_world   = glGetUniformLocation(program, "uni_world");
      fill->uni_diffuse = glGetUniformLocation(program, "uni_map_diffuse");
    }
    
    /* input */
    {
      GLuint program = ctx->graphics_api.gbuffer_fill.program;
      
      GLint pos = glGetAttribLocation(program, "vs_position0");
      
      ctx->graphics_api.gbuffer_fill.input[0].loc = pos;
      ctx->graphics_api.gbuffer_fill.input[0].normalize = GL_FALSE;
      ctx->graphics_api.gbuffer_fill.input[0].size = 8 * sizeof(GLfloat);
      ctx->graphics_api.gbuffer_fill.input[0].component_count = 3;
      ctx->graphics_api.gbuffer_fill.input[0].ptr = 0;
      
      GLint texc = glGetAttribLocation(program, "vs_texcoord0");
      
      ctx->graphics_api.gbuffer_fill.input[1].loc = texc;
      ctx->graphics_api.gbuffer_fill.input[1].normalize = GL_FALSE;
      ctx->graphics_api.gbuffer_fill.input[1].size = 8 * sizeof(GLfloat);
      ctx->graphics_api.gbuffer_fill.input[1].component_count = 2;
      ctx->graphics_api.gbuffer_fill.input[1].ptr = (void*)(3 * sizeof(GLfloat));

      GLint norm = glGetAttribLocation(program, "vs_normal0");

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
        char file[1024];
        memset(file, 0, sizeof(file));

        strcat(file, roa_exe_dir());
        strcat(file, "assets/shaders/");
        strcat(file, "decal.vert");

        const char *vs = 0;
        roa_copy_file(file, (char **)&vs, ROA_NULL);
         
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
        char file[1024];
        memset(file, 0, sizeof(file));

        strcat(file, roa_exe_dir());
        strcat(file, "assets/shaders/");
        strcat(file, "decal.frag");

        const char *fs = 0;
        roa_copy_file(file, (char**)&fs, ROA_NULL);
          
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
        
        glrObjectLabel(GL_PROGRAM, prog, "Decal");

        glDeleteShader(vert_shd);
        glDeleteShader(frag_shd);

        ctx->graphics_api.decal.program = prog; 
      }
    }
    
    /* input */
    {
      struct ogl_decal *decal = &ctx->graphics_api.decal;
      GLuint program = decal->program;
      
      GLint pos = glGetAttribLocation(program, "vs_position0");
      
      decal->input[0].loc             = pos;
      decal->input[0].normalize       = GL_FALSE;
      decal->input[0].size            = 9 * sizeof(GLfloat);
      decal->input[0].component_count = 4;
      decal->input[0].ptr             = 0;
      
      GLint texc = glGetAttribLocation(program, "vs_texcoord0");
      
      decal->input[1].loc             = texc;
      decal->input[1].normalize       = GL_FALSE;
      decal->input[1].size            = 9 * sizeof(GLfloat);
      decal->input[1].component_count = 2;
      decal->input[1].ptr             = (void*)(4 * sizeof(GLfloat));

      GLint norm = glGetAttribLocation(program, "vs_normal0");

      decal->input[2].loc             = norm;
      decal->input[2].normalize       = GL_TRUE;
      decal->input[2].size            = 9 * sizeof(GLfloat);
      decal->input[2].component_count = 3;
      decal->input[2].ptr             = (void*)(6 * sizeof(GLfloat));
    }

    /* uniforms */
    {
      struct ogl_decal *decal = &ctx->graphics_api.decal;
      GLuint program = decal->program;

      decal->uni_world_pos    = glGetUniformLocation(program, "uni_map_worldpos");
      decal->uni_depth        = glGetUniformLocation(program, "uni_map_depth");
      decal->uni_diffuse      = glGetUniformLocation(program, "uni_map_diffuse");
      decal->uni_view         = glGetUniformLocation(program, "uni_view");
      decal->uni_proj         = glGetUniformLocation(program, "uni_proj");
      decal->uni_world        = glGetUniformLocation(program, "uni_world");
      decal->uni_inv_projview = glGetUniformLocation(program, "uni_inv_proj_view");
      decal->uni_inv_world    = glGetUniformLocation(program, "uni_inv_world");
      decal->uni_color        = glGetUniformLocation(program, "uni_color");
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

      glrObjectLabel(GL_BUFFER, vbo, "Decal:Volume(P3N3T2)");

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
        char file[1024];
        memset(file, 0, sizeof(file));

        strcat(file, roa_exe_dir());
        strcat(file, "assets/shaders/");
        strcat(file, "blit.vert");

        const char *vs = 0;
        roa_copy_file(file, (char**)&vs, ROA_NULL);

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
        char file[1024];
        memset(file, 0, sizeof(file));

        strcat(file, roa_exe_dir());
        strcat(file, "assets/shaders/");
        strcat(file, "blit.frag");

        const char *fs = 0;
        roa_copy_file(file, (char**)&fs, ROA_NULL);

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

        glrObjectLabel(GL_PROGRAM, prog, "FullscreenBlit");

        glDeleteShader(vert_shd);
        glDeleteShader(frag_shd);

        ctx->graphics_api.blit.program = prog;
      }
    }

    /* input */
    {
      struct ogl_blit_pass *blit = &ctx->graphics_api.blit;
      GLuint program = blit->program;

      GLint pos = glGetAttribLocation(program, "vs_position0");

      blit->input[0].loc              = pos;
      blit->input[0].normalize        = GL_FALSE;
      blit->input[0].size             = 4 * sizeof(GLfloat);
      blit->input[0].component_count  = 2;
      blit->input[0].ptr              = 0;

      GLint texc = glGetAttribLocation(program, "vs_texcoord0");

      blit->input[1].loc             = texc;
      blit->input[1].normalize       = GL_FALSE;
      blit->input[1].size            = 4 * sizeof(GLfloat);
      blit->input[1].component_count = 2;
      blit->input[1].ptr             = (void*)(2 * sizeof(GLfloat));
    }
    
    /* uniforms */
    {
      struct ogl_blit_pass *blit = &ctx->graphics_api.blit;
      GLuint program = blit->program;

      blit->uni_blit_src = glGetUniformLocation(program, "uni_map_color");
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
    
      glrObjectLabel(GL_BUFFER, vbo, "FullscreenTriangle(f2:f2)");
    
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

    glrObjectLabel(GL_BUFFER, vbo, "Temp:Cube");

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
