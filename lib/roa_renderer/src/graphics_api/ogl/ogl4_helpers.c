#ifdef ROA_RENDERER_API_GL4

#include <roa_renderer/roa_renderer.h>
#include <graphics_api/ogl/ogl4_helpers.h>
#include <roa_lib/assert.h>


/* ------------------------------------------------------- [ OGL Helpers ] -- */


void
glrPushMarkerGroup(const char *marker)
{
  if(OGL4_DEBUG_LABLES && glPushDebugGroup) {
    glPushDebugGroup(GL_DEBUG_SOURCE_APPLICATION, GL_DEBUG_TYPE_PUSH_GROUP, -1, marker);
  }
}


void
glrPopMarkerGroup()
{
  if(OGL4_DEBUG_LABLES && glPopDebugGroup) {
    glPopDebugGroup();
  }
}


void
glrObjectLabel(GLenum type, GLuint item, const char *name)
{
  if (OGL4_DEBUG_LABLES && glObjectLabel) {
    glObjectLabel(type, item, -1, name);
  }
}


GLenum stride_to_gl_index_type(int i)
{
  switch(i)
  {
    case(sizeof(unsigned char)): return GL_UNSIGNED_BYTE;
    case(sizeof(unsigned short)): return GL_UNSIGNED_SHORT;
    case(sizeof(unsigned int)): return GL_UNSIGNED_INT;
  }
  ROA_ASSERT(0);
  return sizeof(unsigned int);
}


GLsizei
gl_type_size(GLenum i)
{
  switch (i)
  {
    case(GL_UNSIGNED_BYTE): return sizeof(GLubyte);
    case(GL_UNSIGNED_INT): return sizeof(GLuint);
    case(GL_UNSIGNED_SHORT): return sizeof(GLushort);
  }

  ROA_ASSERT(0);
  return 0;
}


#endif /* api guard */
