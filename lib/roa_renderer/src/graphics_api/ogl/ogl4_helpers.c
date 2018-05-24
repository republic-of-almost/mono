#ifdef ROA_RENDERER_API_GL4


#include <graphics_api/ogl/ogl4_helpers.h>


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


#endif /* api guard */
