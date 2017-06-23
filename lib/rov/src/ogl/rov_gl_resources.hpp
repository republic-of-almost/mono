#ifndef ROV_GL_RESOURCES_INCLUDED_5BA7BFD7_4B62_4071_B0F3_B736A3D5F5DC
#define ROV_GL_RESOURCES_INCLUDED_5BA7BFD7_4B62_4071_B0F3_B736A3D5F5DC


#include <rov/rov.hpp>
#include "rov_gl.hpp"
#include <stdint.h>
#include <stddef.h>


namespace ROV_Internal {



// -------------------------------------------------------------- [ Texture ] --


uint32_t    ogl_createTexture(
              rovGLData *gl_data,
              uint8_t *data,
              uint32_t width,
              uint32_t height,
              size_t size,
              uint32_t format,
              uintptr_t *out_resource_id = nullptr);


// ----------------------------------------------------------------- [ Mesh ] --


uint32_t    ogl_createMesh(
              rovGLData *gl_data,
              const float *pos,
              const float *normals,
              const float *tex_coords,
              size_t count,
              uintptr_t *out_resource_id = nullptr);


// ------------------------------------------------------------- [ Lighting ] --


uint32_t    ogl_createLights(
              rovGLData *gl_data,
              rovLight *lights,
              size_t count,
              uintptr_t *out_resource_id = nullptr);



// ------------------------------------------------------------- [ Programs ] --


bool        ogl_createProgram(
              const char *filename,
              GLuint *out_vs,
              GLuint *out_gs,
              GLuint *out_fs,
              GLuint *out_program);


} // ns


#endif // inc guard
