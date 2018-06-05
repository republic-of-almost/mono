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


GLenum roa_to_gl(int i)
{
  switch(i)
  {
    case(ROA_RENDERER_TYPE_BYTE): return GL_BYTE;
    case(ROA_RENDERER_TYPE_UBYTE): return GL_UNSIGNED_BYTE;
    case(ROA_RENDERER_TYPE_SHORT): return GL_SHORT;
    case(ROA_RENDERER_TYPE_USHORT): return GL_UNSIGNED_SHORT;
    case(ROA_RENDERER_TYPE_UINT): return GL_UNSIGNED_INT;
    case(ROA_RENDERER_TYPE_FLOAT): return GL_FLOAT;
  }
  ROA_ASSERT(0);
  return GL_UNSIGNED_INT;
}


#endif /* api guard */
