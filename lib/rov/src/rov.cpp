#include <rov/rov.hpp>

#ifdef ROV_GL4
#include "ogl/rov_gl.hpp"
#include "ogl/rov_gl_resources.hpp"
#include "ogl/rov_gl_exec.hpp"
#endif

#ifdef ROV_NOP
#include "nop/rov_nop.hpp"
#endif

#include "rov_data.hpp"
#include <lib/color.hpp>
#include <lib/assert.hpp>


namespace {

struct ROV_data {
  
  #ifdef ROV_GL4
  ROV_Internal::rovGLData gl_data;
  #endif
  
  #ifdef ROV_NOP
  ROV_Internal::rovNopData nop_data;
  #endif
  
  uint32_t graphics_api = 0;
  
  ROV_Internal::rovData rov_data;
};

ROV_data&
get_rov_data()
{
  static ROV_data data;
  return data;
};

} // anon ns


// ------------------------------------------------------------- [ Lifetime ] --


void
rov_initialize(const uint32_t graphics_api, const char *asset_prefix)
{
  get_rov_data().graphics_api = graphics_api;

  switch(graphics_api)
  {
    #ifdef ROV_GL4
    case(rovGraphicsApi_GL4):
      ROV_Internal::ogl_init(&get_rov_data().gl_data, asset_prefix);
      break;
    #endif
    
    #ifdef ROV_NOP
    case(rovGraphicsApi_NOP):
      ROV_Internal::nop_init();
      break;
    #endif
  }
}


void
rov_destroy()
{
  switch(get_rov_data().graphics_api)
  {
    #ifdef ROV_GL4
    case(rovGraphicsApi_GL4):
      ROV_Internal::ogl_destroy(&get_rov_data().gl_data);
      break;
    #endif
    
    #ifdef ROV_NOP
    case(rovGraphicsApi_NOP):
      ROV_Internal::nop_destroy();
    #endif
  }
}


void
rov_execute()
{
  switch(get_rov_data().graphics_api)
  {
    #ifdef ROV_GL4
    case(rovGraphicsApi_GL4):
      ROV_Internal::ogl_exec(&get_rov_data().gl_data, &get_rov_data().rov_data);
      break;
    #endif
    
    #ifdef ROV_NOP
    case(rovGraphicsApi_NOP):
      ROV_Internal::nop_exec();
      break;
    #endif
  }
  
  get_rov_data().rov_data.rov_render_passes.clear();
}


// ------------------------------------------------------------ [ Resources ] --


uint32_t
rov_createTexture(
  uint8_t *data,
  uint32_t width,
  uint32_t height,
  size_t size,
  uint32_t format,
  uintptr_t *out_platform_resource)
{
  switch(get_rov_data().graphics_api)
  {
    #ifdef ROV_GL4
    case(rovGraphicsApi_GL4):
      return ROV_Internal::ogl_createTexture(
        &get_rov_data().gl_data,
        data,
        width,
        height,
        size,
        format,
        out_platform_resource
      );
    #endif
    
    #ifdef ROV_NOP
    case(rovGraphicsApi_NOP):
      return ROV_Internal::nop_createTexture();
    #endif
  }
  
  return false;
}


uint32_t
rov_createMesh(
  const float *pos,
  const float *normals,
  const float *tex_coords,
  size_t count,
  uintptr_t *out_platform_resource)
{
  switch(get_rov_data().graphics_api)
  {
    #ifdef ROV_GL4
    case(rovGraphicsApi_GL4):
      return ROV_Internal::ogl_createMesh(
        &get_rov_data().gl_data,
        pos,
        normals,
        tex_coords,
        count,
        out_platform_resource
      );
    #endif
    
    #ifdef ROV_NOP
    case(rovGraphicsApi_NOP):
      return ROV_Internal::nop_createMesh();
    #endif
  }
  
  LIB_ASSERT(false); // Why no api?
  return 0;
}

uint32_t
rov_createIndex(
  const uint32_t *index,
  size_t index_count,
  uintptr_t *out_platform_resource)
{
  switch(get_rov_data().graphics_api)
  {
    #ifdef ROV_GL4
    case(rovGraphicsApi_GL4):
      return ROV_Internal::ogl_createIndex(
        &get_rov_data().gl_data,
        index,
        index_count,
        out_platform_resource
      );
    #endif
    
    #ifdef ROV_NOP
    case(rovGraphicsApi_NOP):
      return ROV_Internal::nop_createIndex();
    #endif
  }
  
  LIB_ASSERT(false); // Why no api?
  return 0;
}


uint32_t
rov_createLights(
  rovLight *lights,
  size_t count)
{
  switch(get_rov_data().graphics_api)
  {
    #ifdef ROV_GL4
    case(rovGraphicsApi_GL4):
      return ROV_Internal::ogl_createLights(
        &get_rov_data().gl_data,
        lights,
        count
      );
    #endif
    
    #ifdef ROV_NOP
    case(rovGraphicsApi_NOP):
      return ROV_Internal::nop_createLights();
    #endif
  }
  
  LIB_ASSERT(false); // why no api?
  return 0;
}


bool
rov_updateLights(
  uint32_t id,
  rovLight *lights,
  size_t count)
{
  switch(get_rov_data().graphics_api)
  {
    #ifdef ROV_GL4
    case(rovGraphicsApi_GL4):
      return ROV_Internal::ogl_updateLights(
        &get_rov_data().gl_data,
        id,
        lights,
        count
      );
    #endif
    
    #ifdef ROV_NOP
    case(rovGraphicsApi_NOP):
      return ROV_Internal::nop_updateLights();
    #endif
  }
  
  LIB_ASSERT(false); // Why no api?
  return 0;
}


uint32_t
rov_createRenderTarget(
  uint32_t width,
  uint32_t height,
  uint32_t format,
  uintptr_t *out_platform_resource
)
{
  switch(get_rov_data().graphics_api)
  {
    #ifdef ROV_GL4
    case(rovGraphicsApi_GL4):
      return ROV_Internal::ogl_createFramebuffer(
        &get_rov_data().gl_data,
        width,
        height, 
        format,
        out_platform_resource
      );
    #endif
    
    #ifdef ROV_NOP
    case(rovGraphicsApi_NOP):
      return ROV_Internal::nop_createRenderTarget();
    #endif
  }
  
  LIB_ASSERT(false); // Why no api?
  return 0;
}


// ----------------------------------------------------------- [ Renderpass ] --


void
rov_startRenderPass(
  const rovMat4 view,
  const rovMat4 proj,
  const rovVec3 eye_pos,
  const rovViewport viewport,
  uint32_t clear_flags,
  uint32_t light_buffer,
  uint32_t render_target)
{
  ROV_data &data = get_rov_data();

  std::vector<ROV_Internal::rovRenderPass> &rps{
    data.rov_data.rov_render_passes
  };

  rps.emplace_back();
  ROV_Internal::rovRenderPass *rp = &rps.back();

  rp->light_buffer  = light_buffer;
  rp->render_target = render_target;

  memcpy(rp->view, view, sizeof(rovMat4));
  memcpy(rp->proj, proj, sizeof(rovMat4));
  memcpy(rp->viewport, viewport, sizeof(rovViewport));
  memcpy(rp->eye_position, eye_pos, sizeof(rovVec3));

  rp->clear_color = lib::color::init(get_rov_data().rov_data.curr_rov_color);
  rp->clear_flags = clear_flags;
  rp->rasterizer  = data.rov_data.curr_rov_rasterizer;

  rp->materials.emplace_back(rov_curr_material(&get_rov_data().rov_data), size_t{0});
}


// ----------------------------------------------------- [ General Settings ] --


void
rov_resetState()
{
//  get_rov_data().rov_data = ROV_Internal::rovData{};
  
  get_rov_data().rov_data.curr_rov_rasterizer = rovRasterizer{};
  memset(get_rov_data().rov_data.curr_rov_clear_color, 0, sizeof(get_rov_data().rov_data.curr_rov_clear_color));
  memset(get_rov_data().rov_data.curr_rov_color, 0xFF, sizeof(get_rov_data().rov_data.curr_rov_color));
  memset(get_rov_data().rov_data.curr_rov_textures, 0, sizeof(get_rov_data().rov_data.curr_rov_textures));
  get_rov_data().rov_data.curr_rov_mesh = 0;
  get_rov_data().rov_data.curr_rov_index = 0;
  get_rov_data().rov_data.curr_rov_mesh_shader = rovShader_Fullbright;
  get_rov_data().rov_data.curr_rov_framebuffer = 0;
}


void
rov_setRasterizer(rovRasterizer rasterizer)
{
  get_rov_data().rov_data.curr_rov_rasterizer = rasterizer;
}


void
rov_setRenderTarget(const uint32_t render_target)
{
  get_rov_data().rov_data.curr_rov_framebuffer = render_target;
}


void
rov_setColor(const float col[4])
{
  rov_setColor(col[0], col[1], col[2], col[3]);
}


void
rov_setColor(float r, float g, float b, float a)
{
  get_rov_data().rov_data.curr_rov_color[0] = r;
  get_rov_data().rov_data.curr_rov_color[1] = g;
  get_rov_data().rov_data.curr_rov_color[2] = b;
  get_rov_data().rov_data.curr_rov_color[3] = a;
}


void
rov_setColor(uint32_t col)
{
  get_rov_data().rov_data.curr_rov_color[0] = lib::color::get_channel_1f(col);
  get_rov_data().rov_data.curr_rov_color[1] = lib::color::get_channel_2f(col);
  get_rov_data().rov_data.curr_rov_color[2] = lib::color::get_channel_3f(col);
  get_rov_data().rov_data.curr_rov_color[3] = lib::color::get_channel_4f(col);
}


void
rov_setTexture(uint32_t texture_id, uint32_t texture_slot)
{
  if(texture_slot < ROV_Internal::rov_max_textures)
  {
    get_rov_data().rov_data.curr_rov_textures[texture_slot] = texture_id;
  }
}


void
rov_setMesh(uint32_t mesh_id)
{
  get_rov_data().rov_data.curr_rov_mesh = mesh_id;
}


void
rov_setIndex(uint32_t index_id)
{
  get_rov_data().rov_data.curr_rov_index = index_id;
}


void
rov_setShader(uint32_t shader_type)
{
  // -- Param Check -- //
  LIB_ASSERT(shader_type < rovShader_Count);

  get_rov_data().rov_data.curr_rov_mesh_shader = shader_type;
}


// ---------------------------------------------------- [ Dbg Line Renderer ] --


void
rov_submitLine(const float start[3], const float end[3])
{
  ROV_Internal::rovLineDrawCall dc;
  memcpy(dc.start, start, sizeof(rovVec3));
  memcpy(dc.end, end, sizeof(rovVec3));
  memcpy(dc.color, get_rov_data().rov_data.curr_rov_color, sizeof(rovVec3));

  ROV_Internal::rovRenderPass &rp = get_rov_data().rov_data.rov_render_passes.back();

  rp.line_draw_calls.emplace_back(dc);
}


// -------------------------------------------------------- [ Mesh Renderer ] --


void
rov_submitMeshTransform(const float world[16])
{
  ROV_Internal::rovDrawCall dc;
  dc.mesh = get_rov_data().rov_data.curr_rov_mesh;
  dc.index = get_rov_data().rov_data.curr_rov_index;
  memcpy(dc.world, world, sizeof(float) * 16);

  size_t draw_calls = 0;

  ROV_Internal::rovMaterial new_mat{};
  new_mat.draw_calls = 1;
  new_mat.material = rov_curr_material(&get_rov_data().rov_data);

  ROV_Internal::rovRenderPass &rp = get_rov_data().rov_data.rov_render_passes.back();

  const size_t mat_count = rp.materials.size();
  ROV_Internal::rovMaterial *mat = rp.materials.data();

  for(size_t i = 0; i < mat_count; ++i)
  {
    // Found a same material
    if(mat[i].material == new_mat.material)
    {
      mat[i].draw_calls += 1;

      ROV_Internal::rovRenderPass &rp = get_rov_data().rov_data.rov_render_passes.back();
      rp.draw_calls.insert(draw_calls, dc);
      break;
    }

    // Found insert point.
    if(mat[i].material > new_mat.material)
    {
      ROV_Internal::rovRenderPass &rp = get_rov_data().rov_data.rov_render_passes.back();

      rp.materials.insert(i, new_mat);
      rp.draw_calls.insert(draw_calls, dc);
      break;
    }

    draw_calls += mat[i].draw_calls;
  }

  // Insert into the end if we must
  if(get_rov_data().rov_data.rov_render_passes.back().draw_calls.size() == draw_calls)
  {
    ROV_Internal::rovRenderPass &rp = get_rov_data().rov_data.rov_render_passes.back();

    rp.materials.emplace_back(new_mat);
    rp.draw_calls.emplace_back(dc);
  }
}
