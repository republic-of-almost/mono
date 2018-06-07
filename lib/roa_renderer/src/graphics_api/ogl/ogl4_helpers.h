#ifdef ROA_RENDERER_API_GL4

#ifndef OGL4_HELPERS_INCLUDED_B8EE5741_7AE4_46F6_A63C_19D1E6684020
#define OGL4_HELPERS_INCLUDED_B8EE5741_7AE4_46F6_A63C_19D1E6684020


#include <GL/gl3w.h>


/* -------------------------------------------------------- [ OGL Config ] -- */


#ifndef OGL4_ERROR_CHECKS
#ifndef NDEBUG
#define OGL4_ERROR_CHECKS 1
#else
#define OGL4_ERROR_CHECKS 0
#endif
#endif


#ifndef OGL4_DEBUG_LABLES
#ifndef NDEBUG
#define OGL4_DEBUG_LABLES 1
#else
#define OGL4_DEBUG_LABLES 0
#endif
#endif


/* ------------------------------------------------------- [ OGL Helpers ] -- */


void glrPushMarkerGroup(const char *marker);
void glrPopMarkerGroup();

void glrObjectLabel(GLenum type, GLuint item, const char *name);

GLenum stride_to_gl_index_type(int i);
GLsizei gl_type_size(GLenum i);


#endif /* inc guard */

#endif /* api guard */
