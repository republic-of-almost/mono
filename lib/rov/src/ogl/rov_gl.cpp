#ifdef ROV_GL4


#include <rov/rov.hpp>
#include <lib/bench.hpp>
#include <string>
#include <stdio.h>
#include <lib/utilities.hpp>
#include <lib/array.hpp>
#include <math/math.hpp>
#include <GL/gl3w.h>
#include <lib/file.hpp>
#include <lib/string.hpp>
#include <lib/directory.hpp>
#include "../rov_platform_exec.hpp"


#ifdef ROV_GL4
#define GL_HAS_VAO
#define GL_HAS_GEO_SHD
#endif


namespace {

  #ifdef GL_HAS_VAO
  GLuint vao;
  #endif

}


// ------------------------------------------------------------ [ Resources ] --


namespace
{
  struct rovGLTexture
  {
    GLint gl_id;
    uint32_t width;
    uint32_t height;
  };
  
  lib::array<rovGLTexture> rov_textures;
  
  GLint
  format_to_gl_internal(uint32_t format)
  {
    switch(format)
    {
      case(rovPixel_R8):    return GL_R8;
      case(rovPixel_RG8):   return GL_RG8;
      case(rovPixel_RGB8):  return GL_RGB8;
      case(rovPixel_RGBA8): return GL_RGBA8;
      
      default:
        return GL_RGB8;
    }
  }
  
  GLenum
  format_to_gl_format(uint32_t format)
  {
    switch(format)
    {
      case(rovPixel_R8):    return GL_RED;
      case(rovPixel_RG8):   return GL_RG;
      case(rovPixel_RGB8):  return GL_RGB;
      case(rovPixel_RGBA8): return GL_RGBA;
      
      default:
        return GL_RGB;
    }
  }
  
  GLenum
  format_to_gl_type(uint32_t format)
  {
    switch(format)
    {
      case(rovPixel_R8):
      case(rovPixel_RG8):
      case(rovPixel_RGB8):
      case(rovPixel_RGBA8):
        return GL_UNSIGNED_BYTE;
      
      default:
        return GL_UNSIGNED_BYTE;
    }
  }
}


uint32_t
rov_createTexture(const uint8_t *data, uint32_t width, uint32_t height, size_t size, uint32_t format)
{
  #ifdef GL_HAS_VAO
  glBindVertexArray(vao);
  #endif
  
  GLuint texture;
  glGenTextures(1, &texture);
  
  glBindTexture(GL_TEXTURE_2D, texture);
  glTexImage2D(GL_TEXTURE_2D,
                     0,
                     format_to_gl_internal(format),
                     width,
                     height,
                     0,
                     format_to_gl_format(format),
                     format_to_gl_type(format),
                     data);
  
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  
  #ifdef GL_HAS_VAO
  glBindVertexArray(0);
  #endif
  
  rovGLTexture tex;
  tex.gl_id  = texture;
  tex.width  = width;
  tex.height = height;
  
  rov_textures.emplace_back(tex);
  
  return rov_textures.size();
}


namespace
{
  struct rovGLMesh
  {
    GLuint gl_id;
    size_t vertex_stride;
    size_t vertex_count;
  };


  lib::array<rovGLMesh> rov_meshes;
}



uint32_t
rov_createMesh(const float *positions, const float *normals, const float *tex_coords, size_t count)
{
  #ifdef GL_HAS_VAO
  glBindVertexArray(vao);
  #endif

  constexpr size_t stride = 3 + 3 + 2;

  lib::array<GLfloat> vertices;
  vertices.resize(count * stride);

  for(size_t i = 0; i < count; ++i)
  {
    const size_t index = stride * i;

    vertices[index + 0] = positions[(i * 3) + 0];
    vertices[index + 1] = positions[(i * 3) + 1];
    vertices[index + 2] = positions[(i * 3) + 2];

    vertices[index + 3] = normals[(i * 3) + 0];
    vertices[index + 4] = normals[(i * 3) + 1];
    vertices[index + 5] = normals[(i * 3) + 2];

    vertices[index + 6] = tex_coords[(i * 2) + 0];
    vertices[index + 7] = tex_coords[(i * 2) + 1];
  }

  GLuint vbo;
  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * vertices.size(), vertices.data(), GL_STATIC_DRAW);

  const rovGLMesh rov_mesh{vbo, 8, count};
  rov_meshes.emplace_back(rov_mesh);

  #ifdef GL_HAS_VAO
  glBindVertexArray(0);
  #endif

  return (uint32_t)rov_meshes.size();
}


// ------------------------------------------------------------- [ Lifetime ] --


namespace
{
  constexpr uint32_t rov_max_texture_maps = 3;

  struct rov_gl_shader
  {
    GLuint program;

    GLint vs_in_pos;
    GLint vs_in_norm;
    GLint vs_in_uv;

    GLint uni_tex[rov_max_texture_maps];
    
    GLint uni_buffer_01;
    GLint uni_light_count;

    GLint uni_wvp;
    GLint uni_world;
    GLint uni_eye;
    GLint uni_color;
  };

  rov_gl_shader rov_mesh_shaders[3]; // Fullbright / Lit / DirLight


  struct rovGLLineShader
  {
    GLuint program;

    GLint uni_wvp;
    GLint uni_line_buffer;
  };

  rovGLLineShader rov_line_shaders[1]; // Line shader

  inline
  void create_mesh_program(
    const char *vs_src,
    const char *gs_src,
    const char *fs_src,
    rov_gl_shader *out)
  {
    auto shd_compiled = [](const GLuint shd_id) -> bool
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

        printf("SHD ERR: %s\n", error_log.data());

        return false;
      }

      return true;
    };

    const GLuint vert_shd = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vert_shd, 1, &vs_src, NULL);
    glCompileShader(vert_shd);

    if(!shd_compiled(vert_shd))
    {
      LIB_ASSERT(false);
      return;
    }

    #ifdef GL_HAS_GEO_SHD
    GLuint geo_shd;
    if(gs_src)
    {
      geo_shd = glCreateShader(GL_GEOMETRY_SHADER);
      glShaderSource(geo_shd, 1, &gs_src, NULL);
      glCompileShader(geo_shd);

      if(!shd_compiled(geo_shd))
      {
        LIB_ASSERT(false);
        return;
      }
    }
    #endif

    const GLuint frag_shd = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(frag_shd, 1, &fs_src, NULL);
    glCompileShader(frag_shd);

    if(!shd_compiled(frag_shd))
    {
      LIB_ASSERT(false);
      return;
    }

    GLuint prog = glCreateProgram();
    glAttachShader(prog, vert_shd);

    if(gs_src)
    {
      glAttachShader(prog, vert_shd);
    }

    glAttachShader(prog, frag_shd);
    glLinkProgram(prog);

    GLint is_linked = 0;

    glGetProgramiv(prog, GL_LINK_STATUS, &is_linked);
    if(is_linked == GL_FALSE)
    {
      GLint max_length = 0;
      glGetShaderiv(prog, GL_INFO_LOG_LENGTH, &max_length);

      // The maxLength includes the NULL character
      lib::array<GLchar, 1024> error_log;
      error_log.resize(max_length);

      glGetProgramInfoLog(prog, max_length, &max_length, &error_log[0]);

      // Provide the infolog in whatever manor you deem best.
      // Exit with failure.

      printf("SHD ERR: %s\n", error_log.data());
    }


    glUseProgram(prog);

    out->vs_in_pos       = glGetAttribLocation(prog, "vs_in_position");
    out->vs_in_norm      = glGetAttribLocation(prog, "vs_in_normal");
    out->vs_in_uv        = glGetAttribLocation(prog, "vs_in_texture_coords");

    out->uni_tex[0]      = glGetUniformLocation(prog, "uni_map_01");
    out->uni_tex[1]      = glGetUniformLocation(prog, "uni_map_02");
    out->uni_tex[2]      = glGetUniformLocation(prog, "uni_map_03");
    
    out->uni_buffer_01   = glGetUniformLocation(prog, "uni_light_array");
    out->uni_light_count = glGetUniformLocation(prog, "uni_light_count");

    out->uni_wvp         = glGetUniformLocation(prog, "uni_wvp");
    out->uni_world       = glGetUniformLocation(prog, "uni_world");
    out->uni_eye         = glGetUniformLocation(prog, "uni_eye");
    out->uni_color       = glGetUniformLocation(prog, "uni_color");

    out->program = prog;
  }
}


namespace
{
  GLuint dummy_tex = 0; // dummy buffer for
  size_t dummy_count = 0;
}


void
rov_initialize()
{
  rmt_BindOpenGL();

  #ifdef GL_HAS_VAO
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);
  #endif
  
  // Fullbright shader
  {
    char path[2048]{};
    const char * exe_path = lib::dir::exe_path();
    strcat(path, exe_path);
    strcat(path, "../Resources/assets/rov/ogl/fullbright_ogl.glsl");
    
    char program[2048]{};
    lib::file::get_contents(path, program, sizeof(program));
    
    char vert_src[2048]{};
    lib::string::get_text_between_tags(
      "// VERT_START //",
      "// VERT_END //",
      program,
      vert_src,
      sizeof(vert_src)
    );
    
    char frag_src[2048]{};
    lib::string::get_text_between_tags(
      "// FRAG_START //",
      "// FRAG_END //",
      program,
      frag_src,
      sizeof(frag_src)
    );
    
    create_mesh_program(vert_src, nullptr, frag_src, &rov_mesh_shaders[0]);
  }
  
  /*
    Lit Shader
  */
  {
    char path[2048]{};
    const char * exe_path = lib::dir::exe_path();
    strcat(path, exe_path);
    strcat(path, "../Resources/assets/rov/ogl/lit_ogl.glsl");
    
    char program[1 << 13]{};
    lib::file::get_contents(path, program, sizeof(program));
    
    char vert_src[1 << 13]{};
    lib::string::get_text_between_tags(
      "// VERT_START //",
      "// VERT_END //",
      program,
      vert_src,
      sizeof(vert_src)
    );
    
    char frag_src[1 << 13]{};
    lib::string::get_text_between_tags(
      "// FRAG_START //",
      "// FRAG_END //",
      program,
      frag_src,
      sizeof(frag_src)
    );
    
    create_mesh_program(vert_src, nullptr, frag_src, &rov_mesh_shaders[1]);
  }

  /*
    Dir Light shader
  */
  {
    char path[2048]{};
    const char * exe_path = lib::dir::exe_path();
    strcat(path, exe_path);
    strcat(path, "../Resources/assets/rov/ogl/dir_light_ogl.glsl");
    
    char program[1 << 12]{};
    lib::file::get_contents(path, program, sizeof(program));
    
    char vert_src[1 << 12]{};
    lib::string::get_text_between_tags(
      "// VERT_START //",
      "// VERT_END //",
      program,
      vert_src,
      sizeof(vert_src)
    );
    
    char frag_src[2048]{};
    lib::string::get_text_between_tags(
      "// FRAG_START //",
      "// FRAG_END //",
      program,
      frag_src,
      sizeof(frag_src)
    );
    
    create_mesh_program(vert_src, nullptr, frag_src, &rov_mesh_shaders[2]);
  }
  // -- [ Line Renderer ] --

  /*
    Line Shader
  */
  #ifdef GL_HAS_GEO_SHD
  {
     char path[2048]{};
    const char * exe_path = lib::dir::exe_path();
    strcat(path, exe_path);
    strcat(path, "../Resources/assets/rov/ogl/debug_line_ogl.glsl");
    
    char program[1 << 12]{};
    lib::file::get_contents(path, program, sizeof(program));
    
    char vs[1 << 12]{};
    lib::string::get_text_between_tags(
      "// VERT_START //",
      "// VERT_END //",
      program,
      vs,
      sizeof(vs)
    );
    
    char gs[1 << 12]{};
    lib::string::get_text_between_tags(
      "// GEO_START //",
      "// GEO_END //",
      program,
      gs,
      sizeof(gs)
    );
    
    char fs[2048]{};
    lib::string::get_text_between_tags(
      "// FRAG_START //",
      "// FRAG_END //",
      program,
      fs,
      sizeof(fs)
    );

    // -- Create Shader -- //
    const GLuint vert_shd = glCreateShader(GL_VERTEX_SHADER);
    const char *vs_src = vs;
    glShaderSource(vert_shd, 1, &vs_src, NULL);
    glCompileShader(vert_shd);

    const GLuint geo_shd  = glCreateShader(GL_GEOMETRY_SHADER);
    const char *gs_src = gs;
    glShaderSource(geo_shd, 1, &gs_src, NULL);
    glCompileShader(geo_shd);

    const GLuint frag_shd = glCreateShader(GL_FRAGMENT_SHADER);
    const char *fs_src = fs;
    glShaderSource(frag_shd, 1, &fs_src, NULL);
    glCompileShader(frag_shd);

    rov_line_shaders[0].program = glCreateProgram();
    LIB_ASSERT(rov_line_shaders[0].program);

    glAttachShader(rov_line_shaders[0].program, vert_shd);
    glAttachShader(rov_line_shaders[0].program, geo_shd);
    glAttachShader(rov_line_shaders[0].program, frag_shd);

    glLinkProgram(rov_line_shaders[0].program);

    // -- Get Uniforms -- //
    rov_line_shaders[0].uni_wvp = glGetUniformLocation(rov_line_shaders[0].program, "uni_wvp_mat");
    LIB_ASSERT(rov_line_shaders[0].uni_wvp > -1);

    rov_line_shaders[0].uni_line_buffer = glGetUniformLocation(rov_line_shaders[0].program, "uni_line[0]");
    LIB_ASSERT(rov_line_shaders[0].uni_line_buffer > -1);

    glUseProgram(rov_line_shaders[0].program);

    glEnable(GL_DEPTH_TEST);

    glBindVertexArray(0);
  }
  #endif

  // DUMMY LIGHT

  rovLight light_data[] = {
    { {0.f,5.f,0.f}, {1,1,0}, 0.f, 4.f, 4.f, 0.01f, 0.14f, 0.07f },
  };
  
  GLuint dummy_buffer;
  glGenTextures(1, &dummy_buffer);
  
  glBindTexture(GL_TEXTURE_1D, dummy_buffer);
  glTexImage1D(GL_TEXTURE_1D,
                     0,
                     GL_RGBA32F,
                     512,
                     0,
                     GL_RGBA,
                     GL_FLOAT,
                     light_data);
  
  glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  
  dummy_tex = dummy_buffer;
}


void
rov_destroy()
{

}


namespace ROV_Internal {


uint32_t
api_light_buffer(const rovLight lights[], const size_t count)
{
  dummy_count = count;
  
  glBindTexture(GL_TEXTURE_1D, dummy_tex);
  glTexSubImage1D(GL_TEXTURE_1D,
                0,
                0,
                count * 4,
                GL_RGBA,
                GL_FLOAT,
                lights);
  
  return 1;
}


void
api_execute(const rovRenderPass passes[], const size_t pass_count)
{
  #ifdef GL_HAS_VAO
  glBindVertexArray(vao);
  #endif

  glUseProgram(0);
  glDisable(GL_BLEND);
//  glDisable(GL_CULL_FACE);
  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);
  glEnable(GL_DEPTH_TEST);

  /*
    For each renderpass.
  */
  for(size_t p = 0; p < pass_count; ++p)
  {
    const rovRenderPass &rp = passes[p];
  
    GLbitfield cl_flags = 0;
    if(rp.clear_flags & rovClearFlag_Color) { cl_flags |= GL_COLOR_BUFFER_BIT; }
    if(rp.clear_flags & rovClearFlag_Depth) { cl_flags |= GL_DEPTH_BUFFER_BIT; }

    glClearColor(1, 1, 0, 1);
    glClear(cl_flags);
    glEnable(GL_DEPTH_TEST);
    glViewport(rp.viewport[0], rp.viewport[1], rp.viewport[2], rp.viewport[3]);

    const math::mat4 proj = math::mat4_init_with_array(rp.proj);
    const math::mat4 view = math::mat4_init_with_array(rp.view);
    const math::mat4 view_proj = math::mat4_multiply(view, proj);

    /*
      For each material in the pass.
    */
    size_t dc_index = 0;
    
    BENCH_PUSH_GPU(MeshRender);
    
    GLuint last_shd = 0;

    for(auto &mat : rp.materials)
    {
      if(mat.draw_calls == 0)
      {
        continue;
      }
    
      // Pull Material Info Out
      const uint32_t color   = lib::bits::upper32(mat.material);

      const float colorf[4] {
        math::to_float(lib::bits::first8(color)) / 255.f,
        math::to_float(lib::bits::second8(color)) / 255.f,
        math::to_float(lib::bits::third8(color)) / 255.f,
        math::to_float(lib::bits::forth8(color)) / 255.f,
      };

      const uint32_t details = lib::bits::lower32(mat.material);

      const uint8_t shader = lib::bits::first8(details);
      const uint8_t texture_01 = lib::bits::second8(details);
      const uint8_t texture_02 = lib::bits::third8(details);
      const uint8_t texture_03 = lib::bits::forth8(details);

      const rov_gl_shader shd = rov_mesh_shaders[shader];

      /*
        Bind the shader
      */
      if(shd.program != last_shd)
      {
        last_shd = shd.program;
        glUseProgram(shd.program);
      }
      
      
      /*
        Load the textures and buffers
      */
      {
        uint32_t texture_slots = 0;
        
        /*
          Texture Maps
        */
        const uint8_t texture_maps[] = { texture_01, texture_02, texture_03 };
        
        for(uint8_t t = 0; t < rov_max_texture_maps; ++t)
        {
          if(texture_maps[t] && shd.uni_tex[t] != -1)
          {
            rovGLTexture tex = rov_textures[texture_maps[t] - 1];
            
            glActiveTexture(GL_TEXTURE0 + texture_slots);
            glBindTexture(GL_TEXTURE_2D, tex.gl_id);

            glUniform1i(shd.uni_tex[t], texture_slots);
            
            ++texture_slots;
          }
        }
        
        /*
          Buffers
        */
        if(shd.uni_buffer_01 != -1)
        {
          glUniform1i(shd.uni_light_count, dummy_count);
        
          glActiveTexture(GL_TEXTURE0 + texture_slots);
          glBindTexture(GL_TEXTURE_1D, dummy_tex);

          glUniform1i(shd.uni_buffer_01, texture_slots);
          
          ++texture_slots;
        }
      }

      /*
        For each draw call in the material.
      */
      for(uint32_t i = 0; i < mat.draw_calls; ++i)
      {
        auto &dc = rp.draw_calls[dc_index++];

        rovGLMesh vbo;
        
        if(dc.mesh > 0)
        {
          vbo = rov_meshes[dc.mesh - 1];
          glBindBuffer(GL_ARRAY_BUFFER, vbo.gl_id);
          glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        }
        else
        {
          vbo = rovGLMesh{};
          glBindBuffer(GL_ARRAY_BUFFER, 0);
          glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
          
          continue;
        }
        
        /*
          Load input format
        */
        {
          size_t pointer = 0;

          if(shd.vs_in_pos >= 0)
          {
            glEnableVertexAttribArray(shd.vs_in_pos);
            glVertexAttribPointer(
              shd.vs_in_pos,
              3,
              GL_FLOAT,
              GL_FALSE,
              8 * sizeof(float),
              (void*)pointer
            );
          }

          pointer += (sizeof(float) * 3);

          if(shd.vs_in_norm >= 0)
          {
            glEnableVertexAttribArray(shd.vs_in_norm);
            glVertexAttribPointer(
              shd.vs_in_norm,
              3,
              GL_FLOAT,
              GL_FALSE,
              8 * sizeof(float),
              (void*)pointer
            );
          }

          pointer += (sizeof(float) * 3);

          if(shd.vs_in_uv)
          {
            glEnableVertexAttribArray(shd.vs_in_uv);
            glVertexAttribPointer(
              shd.vs_in_uv,
              2,
              GL_FLOAT,
              GL_FALSE,
              8 * sizeof(float),
              (void*)pointer
            );
          }
        }

        /*
          Apply Matrices and other uniforms.
        */
        {
          const math::mat4 world   = math::mat4_init_with_array(dc.world);
          const math::mat4 wvp_mat = math::mat4_multiply(world, view, proj);
          const math::vec4 pos     = math::mat4_multiply(math::vec4_init(0,0,0,1), view);

          glUniformMatrix4fv(shd.uni_wvp, 1, GL_FALSE, math::mat4_get_data(wvp_mat));
          glUniformMatrix4fv(shd.uni_world, 1, GL_FALSE, math::mat4_get_data(world));
          glUniform3fv(shd.uni_eye, 1, pos.data);
          glUniform4fv(shd.uni_color, 1, colorf);
        }

        /*
          Draw
        */
        glDrawArrays(GL_TRIANGLES, 0, vbo.vertex_count);
   
        {
          auto err = glGetError();
          if(err)
          printf("ERR %d\n",err);
        }
      }
    } // For amts
    
    BENCH_POP_GPU;

    // Line Renderer
    #ifdef GL_HAS_GEO_SHD
    {
      BENCH_SCOPED_GPU(DebugLineRender);
    
      glUseProgram(rov_line_shaders[0].program);
      glBindBuffer(GL_ARRAY_BUFFER, 0);
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
      
      // -- Draw the Batches Of Lines -- //
      const size_t batches = (rp.line_draw_calls.size() / 32) + 1;

      for(size_t i = 0; i < batches; ++i)
      {
        // -- Load Camera -- //
        glUniformMatrix4fv(
          rov_line_shaders[0].uni_wvp,
          1,
          GL_FALSE,
          math::mat4_get_data(view_proj)
        );

        const size_t count(
          ((i + 1) == batches) ?
            rp.line_draw_calls.size() % 32 : 32
        );

        const uint32_t batch_start = i * 32;

        glUniform3fv(
          rov_line_shaders[0].uni_line_buffer,
          count * 3,
          (float*)&rp.line_draw_calls[batch_start]
        );

        glDrawArrays(GL_POINTS, 0, count);
      }
    }
    
    {
      auto err = glGetError();
      if(err)
      printf("ERR %d\n",err);
    }
    
    #endif
  }

  #ifdef GL_HAS_VAO
  glBindVertexArray(0);
  #endif
}


} // ns

#endif // gl 4
