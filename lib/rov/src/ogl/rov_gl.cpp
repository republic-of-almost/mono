#ifdef ROV_GL4

#include "rov_gl.hpp"
#include <rov/rov.hpp>
#include <lib/bench.hpp>
#include <string>
#include <stdio.h>
#include <lib/utilities.hpp>
#include <lib/array.hpp>
#include <math/math.hpp>
#include <lib/file.hpp>
#include <lib/string.hpp>
#include <lib/directory.hpp>

#include "rov_gl_exec.hpp"
#include "rov_gl_resources.hpp"


// ------------------------------------------------------------- [ Lifetime ] --


namespace
{
//  ROV_Internal::rovGLMeshProgram rov_mesh_shaders[3]; // Fullbright / Lit / DirLight
//
//  ROV_Internal::rovGLLineShader rov_line_shaders[1]; // Line shader

  inline
  void create_mesh_program(
    const char *vs_src,
    const char *gs_src,
    const char *fs_src,
    ROV_Internal::rovGLMeshProgram *out)
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


namespace ROV_Internal {


void
ogl_init(rovGLData *gl_data)
{
  rmt_BindOpenGL();

  #ifdef GL_HAS_VAO
  glGenVertexArrays(1, &gl_data->vao);
  glBindVertexArray(gl_data->vao);
  #endif
  
      {
      auto err = glGetError();
      if(err)
      printf("ERR %d\n",err);
    }
  
  // Mesh Shaders //
  {
    rovGLMeshProgram fullbright{};
    const bool fb_success = ogl_createProgram(
      "../Resources/assets/rov/ogl/fullbright_ogl.glsl",
      nullptr,
      nullptr,
      nullptr,
      &fullbright.program
    );
    gl_data->rov_mesh_programs.emplace_back(fullbright);
    
    
    {
      auto err = glGetError();
      if(err)
      printf("ERR %d\n",err);
    }
    
    rovGLMeshProgram lit{};
    const bool lit_success = ogl_createProgram(
      "../Resources/assets/rov/ogl/lit_ogl.glsl",
      nullptr,
      nullptr,
      nullptr,
      &lit.program
    );
    gl_data->rov_mesh_programs.emplace_back(lit);
    
    rovGLMeshProgram dir_light{};
    const bool dir_success = ogl_createProgram(
      "../Resources/assets/rov/ogl/dir_light_ogl.glsl",
      nullptr,
      nullptr,
      nullptr,
      &dir_light.program
    );
    gl_data->rov_mesh_programs.emplace_back(dir_light);
    
        {
      auto err = glGetError();
      if(err)
      printf("ERR %d\n",err);
    }
    
    LIB_ASSERT(fb_success && lit_success && dir_success);
    
    for(rovGLMeshProgram &prog : gl_data->rov_mesh_programs)
    {
      glUseProgram(prog.program);

      prog.vs_in_pos       = glGetAttribLocation(prog.program, "vs_in_position");
      prog.vs_in_norm      = glGetAttribLocation(prog.program, "vs_in_normal");
      prog.vs_in_uv        = glGetAttribLocation(prog.program, "vs_in_texture_coords");

      prog.uni_tex[0]      = glGetUniformLocation(prog.program, "uni_map_01");
      prog.uni_tex[1]      = glGetUniformLocation(prog.program, "uni_map_02");
      prog.uni_tex[2]      = glGetUniformLocation(prog.program, "uni_map_03");
      
      prog.uni_buffer_01   = glGetUniformLocation(prog.program, "uni_light_array");
      prog.uni_light_count = glGetUniformLocation(prog.program, "uni_light_count");

      prog.uni_wvp         = glGetUniformLocation(prog.program, "uni_wvp");
      prog.uni_world       = glGetUniformLocation(prog.program, "uni_world");
      prog.uni_eye         = glGetUniformLocation(prog.program, "uni_eye");
      prog.uni_color       = glGetUniformLocation(prog.program, "uni_color");
    }
  }

    {
      auto err = glGetError();
      if(err)
      printf("ERR %d\n",err);
    }

  // Line Renderer //
  #ifdef GL_HAS_GEO_SHD
  {
    rovGLLineProgram line;
    ogl_createProgram(
      "../Resources/assets/rov/ogl/debug_line_ogl.glsl",
      nullptr,
      nullptr,
      nullptr,
      &line.program
    );

    gl_data->rov_line_programs.emplace_back(line);

    // Get Uniforms //
    gl_data->rov_line_programs[0].uni_wvp = glGetUniformLocation(gl_data->rov_line_programs[0].program, "uni_wvp_mat");
    LIB_ASSERT(gl_data->rov_line_programs[0].uni_wvp > -1);

    gl_data->rov_line_programs[0].uni_line_buffer = glGetUniformLocation(gl_data->rov_line_programs[0].program, "uni_line[0]");
    LIB_ASSERT(gl_data->rov_line_programs[0].uni_line_buffer > -1);

    glUseProgram(gl_data->rov_line_programs[0].program);
  }
  #endif
  
    {
      auto err = glGetError();
      if(err)
      printf("ERR %d\n",err);
    }

  // Light Buffer //
  {
    rovLight light_data[] = {
      { {0.f,5.f,0.f}, {1,1,0}, 0.f, 4.f, 4.f, 0.01f, 0.14f, 0.07f },
    };
    
    ogl_createLights(gl_data, light_data, 1);
  }
  
    {
      auto err = glGetError();
      if(err)
      printf("ERR %d\n",err);
    }
}


void
ogl_destroy(rovGLData *gl_data)
{

}


} // ns

#endif // gl 4
