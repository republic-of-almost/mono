#include <nau/renderers/opengl3.h>
#include <GL/gl3w.h>


/* ------------------------------------------------------------ [ Config ] -- */


#ifndef NAU_GL_MALLOC
#include <stdlib.h>
#define NAU_GL_MALLOC(bytes) malloc(bytes)
#endif


#ifndef NAU_GL_FREE
#include <stdlib.h>
#define NAU_GL_FREE(addr) free(addr)
#endif


#ifndef NAU_GL_ASSERT
#include <assert.h>
#define NAU_GL_ASSERT(expr) assert(expr)
#endif


#ifndef NAU_GL_LOGGING
#ifndef NAU_GL_LOG
#include <stdio.h>
#define NAU_GL_LOG(str) printf("%s\n", str)
#endif
#endif


#ifndef NAU_GL_ZERO_MEM
#include <string.h>
#define NAU_GL_ZERO_MEM(var, type) memset(var, 0, sizeof(type))
#endif


// ----------------------------------------------------------------- [ Data ] --


struct Nau_gl_ctx
{
  GLuint vao      = 0;
  GLuint program  = 0;
  GLuint vbo      = 0;
  GLuint ebo      = 0;
  GLuint font_tex = 0;
  GLint  wvp_mat  = -1;
};


// ------------------------------------------------------------- [ Lifetime ] --


void
nau_gl3_init(Nau_gl_ctx **gl_ctx, Nau_ctx *ui_ctx)
{
  /* new context */
  Nau_gl_ctx *new_gl_ctx = NULL;
  {
    new_gl_ctx = (Nau_gl_ctx*)NAU_GL_MALLOC(sizeof(Nau_gl_ctx));
    NAU_GL_ZERO_MEM(new_gl_ctx, Nau_gl_ctx);
  }
  
  constexpr char vert_src[] = R"GLSL(
    #version 330 core

    in vec3 vs_in_position;
    in vec4 vs_in_color;
    in vec2 vs_in_texc;

    uniform mat4 uni_wvp_mat;

    out vec4 fs_in_color;
    out vec2 fs_in_texc;

    void
    main()
    {
      gl_Position = uni_wvp_mat *vec4(vs_in_position, 1.0);
      // gl_Position = vec4(vs_in_position, 0.0, 1.0);

      fs_in_texc = vs_in_texc;
      fs_in_color = vs_in_color;
    }

  )GLSL";

  const char frag_src[] = R"GLSL(
    #version 330 core

    in vec2 fs_in_texc;
    in vec4 fs_in_color;

    uniform sampler2D font_map;

    out vec4 fs_out_color;

    void
    main()
    {
      vec4 tex_color = vec4(texture(font_map, fs_in_texc).r);
      vec4 final_color = mix(vec4(fs_in_color.rgb, tex_color.a), fs_in_color, fs_in_color.a);

      if(final_color.a < 0.1)
      {
        discard;
      }

      fs_out_color = final_color;
      fs_out_color.a = 0.8;
    }

  )GLSL";

  // -- VAO -- //
  {
    glGenVertexArrays(1, &new_gl_ctx->vao);
    glBindVertexArray(new_gl_ctx->vao);
  }

  // -- Shaders -- //
  {
    auto shd_compiled = [](const GLuint shd_id, const char *name) -> bool
    {
      GLint is_compiled = 0;

      glGetShaderiv(shd_id, GL_COMPILE_STATUS, &is_compiled);
      if(is_compiled == GL_FALSE)
      {
        GLint max_length = 0;
        glGetShaderiv(shd_id, GL_INFO_LOG_LENGTH, &max_length);

        // The maxLength includes the NULL character
//        std::vector<GLchar> error_log;
//        error_log.resize(max_length);

//        glGetShaderInfoLog(shd_id, max_length, &max_length, &error_log[0]);

        // Provide the infolog in whatever manor you deem best.
        // Exit with failure.
        glDeleteShader(shd_id); // Don't leak the shader.

        NAU_GL_LOG("Failed to create shader");
//        printf("SHD ERR: %s %s\n", name, error_log.data());

        return false;
      }

      return true;
    };

    const char *vsrc = vert_src;
    GLuint vert_shd = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vert_shd, 1, &vsrc, NULL);
    glCompileShader(vert_shd);
    if(!shd_compiled(vert_shd, "foo"))
    {
      assert(false);
    }

    const char* fsrc = frag_src;
    GLuint frag_shd = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(frag_shd, 1, &fsrc, NULL);
    glCompileShader(frag_shd);
    if(!shd_compiled(frag_shd, "bar"))
    {
      assert(false);
    }
    
    GLuint program = glCreateProgram();
    glAttachShader(program, vert_shd);
    glAttachShader(program, frag_shd);
    glBindFragDataLocation(program , 0, "fs_out_color");
    glLinkProgram(program);
    
    GLint is_linked = 0;

    glGetProgramiv(program, GL_LINK_STATUS, &is_linked);

    if(is_linked == GL_FALSE)
    {
      GLint max_length = 0;
      glGetProgramiv(program, GL_INFO_LOG_LENGTH, &max_length);

      // The maxLength includes the NULL character
//      std::vector<GLchar> error_log;
//      error_log.resize(max_length);

//      glGetProgramInfoLog(program, max_length, &max_length, &error_log[0]);

      // Provide the infolog in whatever manor you deem best.
      // Exit with failure.

//      printf("PROG ERR: %s\n", error_log.data());
      
      NAU_GL_LOG("Failed to link program");

      return;
    }

    glUseProgram(program);

    new_gl_ctx->program = program;
  }

  // -- Uniform -- //
  {
    new_gl_ctx->wvp_mat = glGetUniformLocation(new_gl_ctx->program, "uni_wvp_mat");
//     bugi_gl3().uni_tex = glGetAttribLocation(shaderProgram, "texcoord");
// glEnableVertexAttribArray(texAttrib);
// glVertexAttribPointer(texAttrib, 2, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), (void*)(5 * sizeof(GLfloat)));

  }

  // -- Input  -- //
  {
    GLint pos_attr = glGetAttribLocation(new_gl_ctx->program, "vs_in_position");
    if(pos_attr != -1)
    {
      glEnableVertexAttribArray(pos_attr);
      glVertexAttribPointer(pos_attr, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), 0);
    }

    GLint color_attr = glGetAttribLocation(new_gl_ctx->program, "vs_in_color");
    if(color_attr != -1)
    {
      glEnableVertexAttribArray(color_attr);
      glVertexAttribPointer(color_attr, 4, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
    }

    GLint tex_attr = glGetAttribLocation(new_gl_ctx->program, "vs_in_texc");
    if(tex_attr != -1)
    {
      glEnableVertexAttribArray(tex_attr);
      glVertexAttribPointer(tex_attr, 2, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), (void*)(7 * sizeof(GLfloat)));
    }
  }

  // -- Err check -- //
  {
    GLint err = glGetError();
    if(err)
    {
      printf("Input ERR: %d\n", err);
    }
    else
    {
      printf("No Err in setup\n");
    }
  }

  // -- VBO -- //
  {
    GLfloat verts[]
    {
      10.f + 500, 10.f + 500,   1.f, 1.f, 0.f, 0.f, 1.f, 0.f, 0.f,
      100.f + 500, 10.f + 500,  1.f, 1.f, 0.f, 0.f, 1.f, 0.f, 0.f,
      100.f + 500, 100.f + 500, 1.f, 1.f, 0.f, 0.f, 1.f, 0.f, 0.f,
      10.f + 500, 100.f + 500,  1.f, 1.f, 0.f, 0.f, 1.f, 0.f, 0.f,
    };

    glGenBuffers(1, &new_gl_ctx->vbo);
    glBindBuffer(GL_ARRAY_BUFFER, new_gl_ctx->vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STREAM_DRAW);

    GLuint elements[]
    {
       0, 1, 2,
       0, 2, 3,
    };

    glGenBuffers(1, &new_gl_ctx->ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,  new_gl_ctx->ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements), elements, GL_STREAM_DRAW);
  }

  // Font map
  {
    uint32_t width = 0;
    uint32_t height = 0;
    uint32_t channels = 0;
    void *data = nullptr;

//    Bugi::get_font_data(&width, &height, &channels, &data);

//    glGenTextures(1, &bugi_gl3().font_tex);
//    glBindTexture(GL_TEXTURE_2D, new_gl_ctx->font_tex);
//
//    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RED, GL_UNSIGNED_BYTE, data);
//
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  }

  /* -- Err check -- */
  {
    GLint err = glGetError();
    if(err)
    {
      printf("GL ERR: %d\n", err);
    }
    else
    {
      printf("No Err in setup\n");
    }
  }
  
  *gl_ctx = new_gl_ctx;
  
  NAU_GL_LOG("NAU GL CTX initalized");
}


void
nau_gl3_render(Nau_gl_ctx *gl_ctx, Nau_ctx *ui_ctx)
{
  glBindVertexArray(gl_ctx->vao);
  
  int vp_width;
  int vp_height;
  nau_get_viewport(ui_ctx, &vp_width, &vp_height);

  /* -- Projection Matrix -- */
  {
    const float ortho[16]
    {
      2.f / (float)vp_width, 0.f,            0.f,          0.f,
      0.f,          2.f / (float)-vp_height, 0.f,          0.f,
      0.f,          0.f,                  2.f / 10.f,   0.f,
      -1.f,         1.f,                  0.f,          1.f,
    };

    glUniformMatrix4fv(gl_ctx->wvp_mat, 1, GL_FALSE, ortho);
  }

  /* -- Vertex -- */
  {
    int vert_count = 0;
    float *vert_data = nullptr;
    nau_get_vbo(ui_ctx, &vert_data, &vert_count);

    glBindBuffer(GL_ARRAY_BUFFER, gl_ctx->vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vert_count, vert_data, GL_STREAM_DRAW);
  }

  /* -- Index -- */
  {
    int idx_count = 0;
    unsigned int *idx_data = nullptr;
    nau_get_idx(ui_ctx, &idx_data, &idx_count);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gl_ctx->ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * idx_count, idx_data, GL_STREAM_DRAW);
  }

  /* -- Input  -- */
  {
    GLint pos_attr = glGetAttribLocation(gl_ctx->program, "vs_in_position");
    if(pos_attr != -1)
    {
      glEnableVertexAttribArray(pos_attr);
      glVertexAttribPointer(pos_attr, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), 0);
    }

    GLint color_attr = glGetAttribLocation(gl_ctx->program, "vs_in_color");
    if(color_attr != -1)
    {
      glEnableVertexAttribArray(color_attr);
      glVertexAttribPointer(color_attr, 4, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
    }

    GLint tex_attr = glGetAttribLocation(gl_ctx->program, "vs_in_texc");
    if(tex_attr != -1)
    {
      glEnableVertexAttribArray(tex_attr);
      glVertexAttribPointer(tex_attr, 2, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), (void*)(7 * sizeof(GLfloat)));
    }
  }

  /* -- Draw -- */
  {
    glViewport(0,0,vp_width, vp_height);
    glScissor(0, 0, vp_width, vp_height);
    
    glClear(GL_DEPTH_BUFFER_BIT);
    glEnable(GL_BLEND);
    glBlendEquation(GL_FUNC_ADD);
//    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
//    glBlendFunc(GL_SRC_COLOR, GL_DST_COLOR);
    
    glDisable(GL_CULL_FACE);
//    glDepthFunc(GL_EQUAL);
//    glEnable(GL_DEPTH_TEST);
    glEnable(GL_SCISSOR_TEST);
    
    int cmd_count = 0;
    Nau_draw_cmd *cmds = nullptr;
    nau_get_cmds(ui_ctx, &cmds, &cmd_count);

    for(size_t i = 0; i < cmd_count; ++i)
    {
      const Nau_draw_cmd *cmd = &cmds[i];
    
      const GLsizei width  = (cmd->clip[2] - cmd->clip[0]);
      const GLsizei height = (cmd->clip[3] - cmd->clip[1]);

      const GLint x = cmd->clip[0];
      const GLint y = vp_height - cmd->clip[1] - (height);

      glScissor(x, y, width, height);
      uint32_t *idx_start = 0;
      idx_start += cmds[i].offset;
      glDrawElements(GL_TRIANGLES, cmd->count, GL_UNSIGNED_INT, (void*)idx_start);
    }

    // -- Err Check -- //
    {
      auto err = glGetError();
      if(err)
      {
        printf("ERR: %d\n", err);
      }
    }
  }
}


void
nau_gl3_destroy(Nau_gl_ctx **gl_ctx)
{
  NAU_GL_LOG("TODO: Release GL Resources");
}


/* ------------------------------------------------------------ [ Config ] -- */


#undef NAU_GL_MALLOC
#undef NAU_GL_FREE
#undef NAU_GL_ASSERT
#undef NAU_GL_LOGGING
#undef NAU_GL_ZERO_MEM
