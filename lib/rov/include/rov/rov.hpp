#ifndef ROV_INCLUDED_7964B68E_F4A3_4F34_BD80_C488CCB98569
#define ROV_INCLUDED_7964B68E_F4A3_4F34_BD80_C488CCB98569


#include <stdint.h>
#include <stddef.h>


// -------------------------------------------------------- [ Types and ID's] --


using rovMat4     = float[16];
using rovVec4     = float[4];
using rovVec3     = float[3];
using rovViewport = uint32_t[4];
using rovColor    = float[4];

constexpr uint32_t rovPixel_R8        = 1;
constexpr uint32_t rovPixel_RG8       = 2;
constexpr uint32_t rovPixel_RGB8      = 3;
constexpr uint32_t rovPixel_RGBA8     = 4;

constexpr uint32_t rovClearFlag_Color = 1;
constexpr uint32_t rovClearFlag_Depth = 2;

constexpr uint32_t rovInputFormat_PNT = 1;

constexpr uint32_t rovShader_Fullbright = 0;
constexpr uint32_t rovShader_Lit        = 1;
constexpr uint32_t rovShader_DirLight   = 2;
constexpr uint32_t rovShader_Count      = 3;


struct rovLight
{
  rovVec4 position;     // x, y, z, type
  rovVec4 color;        // r, g, b, extra
  rovVec4 attenuation;  // const, linear, expo, extra
};


// ------------------------------------------------------------- [ Lifetime ] --


void        rov_initialize();
void        rov_destroy();
void        rov_execute();


// ------------------------------------------------------------ [ Resources ] --


uint32_t    rov_createTexture(
              uint8_t *data,
              uint32_t width,
              uint32_t height,
              size_t size,
              uint32_t format,
              uintptr_t *out_platform_resource = nullptr);


uint32_t    rov_createMesh(
              const float *pos,
              const float *normals,
              const float *tex_coords,
              size_t count,
              uintptr_t *out_platform_resource = nullptr);


uint32_t    rov_createLights(
              rovLight *lights,
              size_t count);


bool        rov_updateLights(
              uint32_t id,
              rovLight *lights,
              size_t count);

// ----------------------------------------------------------- [ Renderpass ] --


void        rov_startRenderPass(
              const rovMat4 view,
              const rovMat4 proj,
              const rovVec3 eye_pos,
              const rovViewport viewport,
              uint32_t clear_flags,
              uint32_t light_buffer = 0);


// ----------------------------------------------------- [ General Settings ] --


void        rov_setColor(const float col[4]);
void        rov_setColor(float r, float g, float b, float a);
void        rov_setCamera(const float view[16], const float proj[16]);
void        rov_setTexture(uint32_t texture_id, uint32_t texture_slot);
void        rov_setMesh(uint32_t mesh_id);
void        rov_setShader(uint32_t shader_id);


// ---------------------------------------------------- [ Dbg Line Renderer ] --


void        rov_submitLine(const float start[3], const float end[3]);


// -------------------------------------------------------- [ Mesh Renderer ] --


void        rov_submitMeshTransform(const float world[16]);



#endif // inc guard
