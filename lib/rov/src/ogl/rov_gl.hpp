#ifdef ROV_GL4
#ifndef ROV_GL_INCLUDED_7F9FD302_5953_4A67_BA19_FF7909998971
#define ROV_GL_INCLUDED_7F9FD302_5953_4A67_BA19_FF7909998971


#include <rov/rov.hpp>
#include <GL/gl3w.h>
#include <lib/array.hpp>
#include <stddef.h>
#include <stdint.h>



#ifdef ROV_GL4
#define GL_HAS_VAO
#define GL_HAS_GEO_SHD
#endif


namespace ROV_Internal {


// -------------------------------------------------------------- [ GL Data ] --


constexpr uint32_t rov_max_texture_maps = 3;


struct rovGLTexture
{
  GLint gl_id;
  uint32_t width;
  uint32_t height;
};


struct rovGLMesh
{
  GLuint gl_id;
  size_t vertex_stride;
  size_t vertex_count;
};


struct rovGLMeshProgram
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
  GLint uni_normal;
  GLint uni_wv;
};


struct rovGLLineProgram
{
  GLuint program;

  GLint uni_wvp;
  GLint uni_line_buffer;
};


struct rovGLLightPack
{
  GLuint gl_id;
  size_t count;
};


/*
  Data held in GL Land
*/
struct rovGLData
{
  GLuint vao;

  lib::array<rovGLTexture>      rov_textures;
  lib::array<rovGLMesh, 128>         rov_meshes;
  lib::array<rovGLLightPack>    light_buffers;
  lib::array<rovGLMeshProgram>  rov_mesh_programs;
  lib::array<rovGLLineProgram>  rov_line_programs;
};


// ---------------------------------------------------------- [ GL Lifetime ] --


void        ogl_init(rovGLData *gl_data, const char *asset_prefix);
void        ogl_destroy(rovGLData *gl_data);


// ---------------------------------------------------------- [ GL Helpers ] --


inline GLint
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

inline GLenum
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

inline GLenum
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



} // ns


#endif // inc guard
#endif // api guard
