#ifdef ROA_RENDERER_API_GL4

#ifndef OGL4_HELPERS_INCLUDED_B8EE5741_7AE4_46F6_A63C_19D1E6684020
#define OGL4_HELPERS_INCLUDED_B8EE5741_7AE4_46F6_A63C_19D1E6684020


#ifndef OGL4_ERROR_CHECKS
#ifndef NDEBUG
#define OGL4_ERROR_CHECKS 1
#else
#define OGL4_ERROR_CHECKS 0
#endif
#endif


void glrPushMarkerGroup(const char *marker);
void glrPopMarkerGroup();



#endif /* inc guard */

#endif /* api guard */
