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


struct ogl_gbuffer_passes
{
  GLuint program_fill;
  GLuint uni_wvp;
  GLuint uni_world;
};


struct graphics_api
{
  /* global VAO */
  GLuint vao;

  /* gbuffer */
  struct ogl_gbuffer gbuffer;
  struct ogl_gbuffer_passes gbuffer_passes;

  GLuint mesh_program;
  GLuint vbo;
};


#endif /* inc guard */
#endif /* platform guard */
