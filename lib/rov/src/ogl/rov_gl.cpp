#ifdef ROV_GL4

#include "rov_gl.hpp"
#include <rov/rov.hpp>
#include <lib/bench.hpp>
#include <string>
#include <stdio.h>
#include <lib/utilities.hpp>
#include <lib/array.hpp>
#include <lib/file.hpp>
#include <lib/string.hpp>
#include <lib/directory.hpp>

#include "rov_gl_exec.hpp"
#include "rov_gl_resources.hpp"


// ------------------------------------------------------------- [ Lifetime ] --


namespace ROV_Internal {


void
ogl_init(rovGLData *gl_data, const char *asset_prefix)
{
//  rmt_BindOpenGL();

  #ifdef GL_HAS_VAO
  glGenVertexArrays(1, &gl_data->vao);
  glBindVertexArray(gl_data->vao);
  #endif

  // Mesh Shaders //
  {
    char path[2048]{};
    strcat(path, asset_prefix);
    strcat(path, "rov/ogl/fullbright_ogl.glsl");

    rovGLMeshProgram fullbright{};
    const bool fb_success = ogl_createProgram(
      path,
      &fullbright.program
    );
    gl_data->rov_mesh_programs.emplace_back(fullbright);

    memset(path, 0, sizeof(path));
    strcat(path, asset_prefix);
    strcat(path, "rov/ogl/mesh_renderer.glsl");

    rovGLMeshProgram lit{};
    const bool lit_success = ogl_createProgram(
      path,
      &lit.program
    );
    gl_data->rov_mesh_programs.emplace_back(lit);

    LIB_ASSERT(fb_success && lit_success);

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
      prog.uni_view        = glGetUniformLocation(prog.program, "uni_view");
      prog.uni_eye         = glGetUniformLocation(prog.program, "uni_eye");
      prog.uni_color       = glGetUniformLocation(prog.program, "uni_color");
      prog.uni_normal      = glGetUniformLocation(prog.program, "uni_normal");
      prog.uni_wv          = glGetUniformLocation(prog.program, "uni_wv");
    }
  }

  // Line Renderer //
  #ifdef GL_HAS_GEO_SHD
  {
    char path[2048]{};
    strcat(path, asset_prefix);
    strcat(path, "rov/ogl/debug_line_ogl.glsl");

    rovGLLineProgram line;
    ogl_createProgram(
      path,
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

  // Light Buffer //
  {
    rovLight light_data[] = {
      {
        {0.f,5.f,0.f,0.f},
        {1.f, 1.f, 1.f, 1.f},
        {0.01f, 0.14f, 0.07f, 0.f},
      }
    };

    ogl_createLights(gl_data, light_data, 1);
  }
}


void
ogl_destroy(rovGLData *gl_data)
{
  #ifdef GL_HAS_VAO
  glBindVertexArray(gl_data->vao);
  #endif
  
  ogl_destroyLights(gl_data);
  ogl_destroyTextures(gl_data);
  ogl_destroyMeshes(gl_data);
  ogl_destroyPrograms(gl_data);

  LOG_TODO("Remove Framebuffers");

  glDeleteVertexArrays(1, &gl_data->vao);
}


} // ns

#endif // gl 4
