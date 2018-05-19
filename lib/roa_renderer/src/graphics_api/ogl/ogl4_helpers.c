#ifdef ROA_RENDERER_API_GL4


#include <graphics_api/ogl/ogl4_helpers.h>
#include <GL/gl3w.h>


void
glrPushMarkerGroup(const char *marker)
{
  if(glPushDebugGroup) {
    glPushDebugGroup(GL_DEBUG_SOURCE_APPLICATION, GL_DEBUG_TYPE_PUSH_GROUP, -1, marker);
  }
}


void
glrPopMarkerGroup()
{
  if(glPopDebugGroup) {
    glPopDebugGroup();
  }
}


#endif /* api guard */
