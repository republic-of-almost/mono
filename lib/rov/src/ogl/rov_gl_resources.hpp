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

void        ogl_destroyTextures(rovGLData *gl_data);


// ----------------------------------------------------------------- [ Mesh ] --


uint32_t    ogl_createMesh(
              rovGLData *gl_data,
              const float *pos,
              const float *normals,
              const float *tex_coords,
              size_t count,
              uintptr_t *out_resource_id = nullptr);


void        ogl_destroyMeshes(rovGLData *gl_data);


// ---------------------------------------------------------------- [ Index ] --


uint32_t    ogl_createIndex(
              rovGLData *gl_data,
              const uint32_t *index,
              const size_t count,
              uintptr_t *out_resource_id = nullptr);

void        ogl_destroyIndexes(rovGLData *gl_data);


// ------------------------------------------------------------- [ Lighting ] --


uint32_t    ogl_createLights(
              rovGLData *gl_data,
              rovLight *lights,
              size_t count,
              uintptr_t *out_resource_id = nullptr);
  
  
bool        ogl_updateLights(
              rovGLData *gl_data,
              uint32_t id,
              rovLight *lights,
              size_t count);


void        ogl_destroyLights(rovGLData *gl_data);


// ------------------------------------------------------------- [ Lighting ] --


uint32_t    ogl_createFramebuffer(
              rovGLData *gl_data,
              uint32_t width,
              uint32_t height,
              uint32_t format,
              uintptr_t *out_platform_resource);


// ------------------------------------------------------------- [ Programs ] --


bool        ogl_createProgram(
              const char *filename,
              GLuint *out_program);

void        ogl_destroyPrograms(rovGLData *gl_data);


} // ns


#endif // inc guard
