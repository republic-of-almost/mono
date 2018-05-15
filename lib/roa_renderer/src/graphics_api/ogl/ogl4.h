#ifdef ROA_RENDERER_API_GL4
#ifndef OGL_CTX_INCLUDED_
#define OGL_CTX_INCLUDED_


#include <GL/gl3w.h>


struct ogl_gbuffer
{
  GLuint fbo;
  GLuint texture_output[4];
  GLuint texture_depth;
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


struct graphics_api
{
  /* global VAO */
  GLuint vao;

  /* gbuffer */
  struct ogl_gbuffer gbuffer;
  struct ogl_gbuffer_fill_pass gbuffer_fill;
  struct ogl_blit_pass blit;

  GLuint mesh_program;
  GLuint vbo;
};


#endif /* inc guard */
#endif /* platform guard */
