#ifdef ROA_RENDERER_API_GL4
#ifndef OGL_CTX_INCLUDED_5774619E_CC5C_43DF_B796_799C8DDFC489
#define OGL_CTX_INCLUDED_5774619E_CC5C_43DF_B796_799C8DDFC489


#include <GL/gl3w.h>


struct ogl_gbuffer
{
  GLuint fbo;
  GLuint texture_output[4];
  GLuint texture_depth;
};


struct ogl_gbuffer_dir_light
{
  GLuint program;
};


struct ogl_gbuffer_fill_pass
{
  GLuint program;
  GLuint uni_wvp;
  GLuint uni_world;
  GLuint uni_colormap;
};


struct ogl_blit_pass
{
  GLuint program;
  GLuint fullscreen_triangle;
};


struct ogl_decal
{
  GLuint program;
  GLuint vbo;
};


struct ogl_mesh
{
  GLuint vbo;
  int vertex_count;

  GLuint ibo;
  int index_count;
};


struct graphics_api
{
  /* global VAO */
  GLuint vao;

  /* gbuffer */
  struct ogl_gbuffer gbuffer;
  struct ogl_gbuffer_fill_pass gbuffer_fill;
  struct ogl_gbuffer_dir_light dir_light;
  struct ogl_blit_pass blit;
  struct ogl_decal decal;

  uint64_t *mesh_ids;
  struct ogl_mesh *meshes;

  GLuint mesh_program;
  GLuint vbo;
  GLuint tex;
};


#endif /* inc guard */
#endif /* platform guard */
