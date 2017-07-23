#include "renderable.hpp"
#include <GL/gl3w.h>
#include <lib/array.hpp>
#include <lib/logging.hpp>
#include <math/math.hpp>
#include <lib/string.hpp>
#include <lib/file.hpp>
#include <lib/assert.hpp>
#include <lib/directory.hpp>

#include <OpenGL/gl3.h>

namespace
{
  struct DrawArraysIndirectCommand
  {
    GLuint count;
    GLuint primCount;
    GLuint first;
    GLuint baseInstance;
  };

  GLuint indirect_draw_buffer;
  GLuint draw_index_buffer;

  GLuint vao;

  GLuint vbo;
  GLuint data_buffer;
  GLuint render_program;

  // This was taken from ROV //
  bool
  ogl_createProgram(
    const char *filename,
    GLuint *out_vs,
    GLuint *out_gs,
    GLuint *out_fs,
    GLuint *out_program)
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
        lib::array<GLchar, 1024> error_log;
        error_log.resize(max_length);

        glGetShaderInfoLog(shd_id, max_length, &max_length, &error_log[0]);

        // Provide the infolog in whatever manor you deem best.
        // Exit with failure.
        glDeleteShader(shd_id); // Don't leak the shader.

        printf("SHD ERR: %s %s\n", name, error_log.data());

        return false;
      }

      return true;
    };

    // -- Load File -- //

    char name[1024]{};
    lib::string::filename_from_path(filename, name);

    char path[2048]{};
//    const char * exe_path = lib::dir::exe_path();
//    strcat(path, exe_path);
    strcat(path, filename);
    
    const size_t bytes = lib::file::bytes(path);

    char program[1 << 17]{};
    lib::file::get_contents(path, program, sizeof(program));

    if(strlen(program) == 0)
    {
      LOG_ERROR("Failed to load shader")
      return false;
    }

    char vert_src[1 << 14]{};
    lib::string::get_text_between_tags(
      "// VERT_START //",
      "// VERT_END //",
      program,
      vert_src,
      sizeof(vert_src)
    );

    char geo_src[1 << 14]{};
    lib::string::get_text_between_tags(
      "// GEO_START //",
      "// GEO_END //",
      program,
      geo_src,
      sizeof(geo_src)
    );

    char frag_src[1 << 14]{};
    lib::string::get_text_between_tags(
      "// FRAG_START //",
      "// FRAG_END //",
      program,
      frag_src,
      sizeof(frag_src)
    );

    // -- Create Shader Stages -- //

    const GLuint vert_shd = glCreateShader(GL_VERTEX_SHADER);
    const char *vs_src = &vert_src[0];
    glShaderSource(vert_shd, 1, &vs_src, NULL);
    glCompileShader(vert_shd);

    if(!shd_compiled(vert_shd, name))
    {
      LIB_ASSERT(false);
      return false;
    }

      {
        auto err = glGetError();
        if(err)
        printf("ERR %d\n",err);
      }

    #ifdef GL_HAS_GEO_SHD
    GLuint geo_shd = 0;
    if(strlen(geo_src))
    {
      geo_shd = glCreateShader(GL_GEOMETRY_SHADER);
      const char *gs_src = &geo_src[0];
      glShaderSource(geo_shd, 1, &gs_src, NULL);
      glCompileShader(geo_shd);

      if(!shd_compiled(geo_shd, name))
      {
        LIB_ASSERT(false);
        return false;
      }
    }
    #endif

    const GLuint frag_shd = glCreateShader(GL_FRAGMENT_SHADER);
    const char *fs_src = &frag_src[0];
    glShaderSource(frag_shd, 1, &fs_src, NULL);
    glCompileShader(frag_shd);
    if(!shd_compiled(frag_shd, name))
    {
      LIB_ASSERT(false);
      return false;
    }

    GLuint prog = glCreateProgram();
    glAttachShader(prog, vert_shd);

    glAttachShader(prog, frag_shd);
    glLinkProgram(prog);

    GLint is_linked = 0;

    glGetProgramiv(prog, GL_LINK_STATUS, &is_linked);

    if(is_linked == GL_FALSE)
    {
      GLint max_length = 0;
      glGetProgramiv(prog, GL_INFO_LOG_LENGTH, &max_length);

      // The maxLength includes the NULL character
      lib::array<GLchar, 1024> error_log;
      error_log.resize(max_length);

      glGetProgramInfoLog(prog, max_length, &max_length, &error_log[0]);

      // Provide the infolog in whatever manor you deem best.
      // Exit with failure.

      return false;
    }

    // -- Return Values -- //

    if(out_vs)      { *out_vs = vert_shd;  }
    if(out_fs)      { *out_fs = frag_shd;  }
    if(out_program) { *out_program = prog; }

    return true;
  }
}

// -- //

enum
{
    NUM_DRAWS           = 50000
};


void
edgeInit()
{
  int i = 0;

  if(gl3wInit() < 0)
  {
    LIB_ASSERT(false);
  }

  printf("OpenGL %s, GLSL %s\n", glGetString(GL_VERSION),
  glGetString(GL_SHADING_LANGUAGE_VERSION));

  glGenVertexArrays(1, &vao);

  // --------------------------------------------------------- [ Load Shader ]

   ogl_createProgram("/Users/PhilCK/Desktop/rep_of_a/app/tiles/assets/multi_shader.glsl",
      nullptr,
      nullptr,
      nullptr,
      &render_program);

  glUseProgram(render_program);
  {
    auto err = glGetError();
    printf("SHADER ERR: %d\n", err);
  }

  // --------------------------------------------------------- [ Load VBO ]

  constexpr float test_cube[]
  {
    -0.5f, -0.5f, -0.5f, 0.f, 0.f, 1.f,
    0.5f, -0.5f, -0.5f, 0.f, 0.f, 1.f,
    0.5f,  0.5f, -0.5f, 0.f, 0.f, 1.f,
    0.5f,  0.5f, -0.5f, 0.f, 0.f, 1.f,
    -0.5f,  0.5f, -0.5f, 0.f, 0.f, 1.f,
    -0.5f, -0.5f, -0.5f, 0.f, 0.f, 1.f,

    -0.5f, -0.5f,  0.5f, 0.f, 0.f, -1.f,
    0.5f, -0.5f,  0.5f, 0.f, 0.f, -1.f,
    0.5f,  0.5f,  0.5f, 0.f, 0.f, -1.f,
    0.5f,  0.5f,  0.5f, 0.f, 0.f, -1.f,
    -0.5f,  0.5f,  0.5f, 0.f, 0.f, -1.f,
    -0.5f, -0.5f,  0.5f, 0.f, 0.f, -1.f,

    -0.5f,  0.5f,  0.5f, 1.f, 0.f, 0.f,
    -0.5f,  0.5f, -0.5f, 1.f, 0.f, 0.f,
    -0.5f, -0.5f, -0.5f, 1.f, 0.f, 0.f,
    -0.5f, -0.5f, -0.5f, 1.f, 0.f, 0.f,
    -0.5f, -0.5f,  0.5f, 1.f, 0.f, 0.f,
    -0.5f,  0.5f,  0.5f, 1.f, 0.f, 0.f,

    0.5f,  0.5f,  0.5f, -1.f, 0.f, 0.f,
    0.5f,  0.5f, -0.5f, -1.f, 0.f, 0.f,
    0.5f, -0.5f, -0.5f, -1.f, 0.f, 0.f,
    0.5f, -0.5f, -0.5f, -1.f, 0.f, 0.f,
    0.5f, -0.5f,  0.5f, -1.f, 0.f, 0.f,
    0.5f,  0.5f,  0.5f, -1.f, 0.f, 0.f,

    -0.5f, -0.5f, -0.5f, 0.f, 1.f, 0.f,
    0.5f, -0.5f, -0.5f, 0.f, 1.f, 0.f,
    0.5f, -0.5f,  0.5f, 0.f, 1.f, 0.f,
    0.5f, -0.5f,  0.5f, 0.f, 1.f, 0.f,
    -0.5f, -0.5f,  0.5f, 0.f, 1.f, 0.f,
    -0.5f, -0.5f, -0.5f, 0.f, 1.f, 0.f,

    -0.5f,  0.5f, -0.5f, 0.f, -1.f, 0.f,
    0.5f,  0.5f, -0.5f, 0.f, -1.f, 0.f,
    0.5f,  0.5f,  0.5f, 0.f, -1.f, 0.f,
    0.5f,  0.5f,  0.5f, 0.f, -1.f, 0.f,
    -0.5f,  0.5f,  0.5f, 0.f, -1.f, 0.f,
    -0.5f,  0.5f, -0.5f, 0.f, -1.f, 0.f,
  };

  constexpr uint32_t cube_mesh_vert_count = 36;

  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(float) * (cube_mesh_vert_count * 6), test_cube, GL_STATIC_DRAW);
  
  {
    auto err = glGetError();
    printf("ERR: %d\n", err);
  }

  size_t pointer = 0;
   {
     glEnableVertexAttribArray(glGetAttribLocation(render_program, "position_3"));
     glVertexAttribPointer(
       glGetAttribLocation(render_program, "normal"),
       3,
       GL_FLOAT,
       GL_FALSE,
       6 * sizeof(float),
       (void*)pointer
     );
   }

  {
    auto err = glGetError();
    printf("INPUT ERR: %d - %d, %d, %d, %d\n", err, glGetAttribLocation(render_program, "position_3"), glGetAttribLocation(render_program, "normal"), render_program, data_buffer);
  }

  pointer += (sizeof(float) * 3);

   {
     glEnableVertexAttribArray(glGetAttribLocation(render_program, "normal"));
     glVertexAttribPointer(
       glGetAttribLocation(render_program, "normal"),
       3,
       GL_FLOAT,
       GL_FALSE,
       6 * sizeof(float),
       (void*)pointer
     );
   }

  // --------------------------------------------------------- [ Load IDB ]

  glGenBuffers(1, &indirect_draw_buffer);
  glBindBuffer(GL_DRAW_INDIRECT_BUFFER, indirect_draw_buffer);
  glBufferData(GL_DRAW_INDIRECT_BUFFER,
               NUM_DRAWS * sizeof(DrawArraysIndirectCommand),
               NULL,
               GL_STATIC_DRAW);



  DrawArraysIndirectCommand * cmd = (DrawArraysIndirectCommand *)
      glMapBufferRange(GL_DRAW_INDIRECT_BUFFER,
                       0,
                       NUM_DRAWS * sizeof(DrawArraysIndirectCommand),
                       GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT);

  for (i = 0; i < NUM_DRAWS; i++)
  {
    cmd[i].first = 0;
    cmd[i].count = 36;
    cmd[i].primCount = 1;
    cmd[i].baseInstance = i;
  }

  glUnmapBuffer(GL_DRAW_INDIRECT_BUFFER);
  
  {
    auto err = glGetError();
    printf("ERR: %d\n", err);
  }

  // -------------------------------------------------- [ Load Index Buffer ]

  glBindVertexArray(vao);

  glGenBuffers(1, &draw_index_buffer);
  glBindBuffer(GL_ARRAY_BUFFER, draw_index_buffer);
  glBufferData(GL_ARRAY_BUFFER,
               NUM_DRAWS * sizeof(GLuint),
               NULL,
               GL_STATIC_DRAW);

  GLuint * draw_index =
      (GLuint *)glMapBufferRange(GL_ARRAY_BUFFER,
                                 0,
                                 NUM_DRAWS * sizeof(GLuint),
                                 GL_MAP_WRITE_BIT |
                                 GL_MAP_INVALIDATE_BUFFER_BIT);

  for (i = 0; i < NUM_DRAWS; i++)
  {
      draw_index[i] = i;
  }

  glUnmapBuffer(GL_ARRAY_BUFFER);
  
  {
    auto err = glGetError();
    printf("ERR: %d\n", err);
  }

  // -------------------------------------------------- [ Setup ]

   glVertexAttribIPointer(10, 1, GL_UNSIGNED_INT, 0, NULL);
   glVertexAttribDivisor(10, 1);
   glEnableVertexAttribArray(10);

   glEnable(GL_DEPTH_TEST);
   glDepthFunc(GL_LEQUAL);

   glEnable(GL_CULL_FACE);

  {
    auto err = glGetError();
    printf("ERR: %d\n", err);
  }

   auto err = glGetError();
   if(err)
   {
     LOG_ERROR("edgeInit Failed");
   }
   else
   {
     LOG_INFO("edgeInit Success")
   }
}


void
edgeExecute(
  edgeCamera *camera,
  size_t cam_count,
  edgeRenderable *renderable,
  size_t renderable_count)
{
  glBindVertexArray(vao);

  static float time = 0;
  time += 0.1f;
  float t = time;

  static const float one = 1.0f;
  static const float black[] = { 0.1f, 0.0f, 0.0f, 0.0f };

  // -------------------------------------------------- [ Viewport ]

  glViewport(0, 0, 800, 600);
  glClearBufferfv(GL_COLOR, 0, black);
  glClearBufferfv(GL_DEPTH, 0, &one);
  
  {
    auto err = glGetError();
    printf("ERR: %d\n", err);
  }

  math::mat4 view = math::mat4_lookat(math::vec3_init(100.0f * math::cos(t * 0.023f), 100.0f * math::cos(t * 0.023f), 300.0f * math::sin(t * 0.037f) - 600.0f), math::vec3_init(0,0,260), math::vec3_normalize(math::vec3_init(0.1f - cosf(t * 0.1f) * 0.3f, 1.0f, 0.0f)));
  math::mat4 proj = math::mat4_projection(800, 600, 0.1, 1000, 0.78);

  glUseProgram(render_program);
  glBindBuffer(GL_DRAW_INDIRECT_BUFFER, indirect_draw_buffer);
  glBindBuffer(GL_ARRAY_BUFFER, draw_index_buffer);
  
  {
    auto err = glGetError();
    printf("ERR: %d\n", err);
  }

  // -------------------------------------------------- [ Load Uniforms ]

  glUniform1f(glGetUniformLocation(render_program, "time"), time);
  glUniformMatrix4fv(glGetUniformLocation(render_program, "view_matrix"), 1, GL_FALSE, view.data);
  glUniformMatrix4fv(glGetUniformLocation(render_program, "proj_matrix"), 1, GL_FALSE, proj.data);

  math::mat4 view_proj = math::mat4_multiply(view, proj);

  glUniformMatrix4fv(glGetUniformLocation(render_program, "viewproj_matrix"), 1, GL_FALSE, view_proj.data);

  {
    auto err = glGetError();
    printf("ERR: %d\n", err);
  }

  // -------------------------------------------------- [ Vertex Info ]


  glVertexAttribIPointer(10, 1, GL_UNSIGNED_INT, 0, NULL);
  glVertexAttribDivisor(10, 1);
  glEnableVertexAttribArray(10);

// -------------------------------------------------- [ Render ]

  gl3wGetProcAddress(

  //if (mode == MODE_MULTIDRAW)
  {
     glMultiDrawArraysIndirect(GL_TRIANGLES, NULL, NUM_DRAWS, 0);
//     glBindBuffer(GL_ARRAY_BUFFER, vbo);
//     glDrawArrays(GL_TRIANGLES, 0, 36);
  }
  //else if (mode == MODE_SEPARATE_DRAWS)
  {
//    int j;
//    for (j = 0; j < NUM_DRAWS; j++)
//    {
//        GLuint first = 0;
//        GLuint count = 36;
//        glDrawArraysInstancedBaseInstance(
//          GL_TRIANGLES,
//          first,
//          count,
//          1, j);
//    }
  }
}
