#ifdef ROA_RENDERER_API_GL4
#ifndef OGL_CTX_INCLUDED_
#define OGL_CTX_INCLUDED_


#include <GL/gl3w.h>


struct graphics_api
{
  GLuint mesh_program;
  GLuint vbo;
  GLuint vao;
};


#endif /* inc guard */
#endif /* platform guard */
