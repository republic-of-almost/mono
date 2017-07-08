#include <rov/rov.hpp>
#include "ogl/rov_gl.hpp"
#include "ogl/rov_gl_resources.hpp"
#include "ogl/rov_gl_exec.hpp"
#include "rov_data.hpp"
#include <lib/assert.hpp>


namespace {

ROV_Internal::rovGLData gl_data;
ROV_Internal::rovData rov_data;

} // anon ns


// ------------------------------------------------------------- [ Lifetime ] --


void
rov_initialize()
{
  ROV_Internal::ogl_init(&gl_data);
}


void
rov_destroy()
{
  ROV_Internal::ogl_destroy(&gl_data);
}


void
rov_execute()
{
  ROV_Internal::ogl_exec(&gl_data, &rov_data);
  rov_data.rov_render_passes.clear();
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
  return ROV_Internal::ogl_createTexture(
    &gl_data,
    data,
    width,
    height,
    size,
    format,
    out_platform_resource
  );
}

uint32_t
rov_createMesh(
  const float *pos,
  const float *normals,
  const float *tex_coords,
  size_t count,
  uintptr_t *out_platform_resource)
{
  return ROV_Internal::ogl_createMesh(
    &gl_data,
    pos,
    normals,
    tex_coords,
    count,
    out_platform_resource
  );
}

uint32_t
rov_createLights(
  rovLight *lights,
  size_t count)
{
  return ROV_Internal::ogl_createLights(
    &gl_data,
    lights,
    count
  );
}


bool
rov_updateLights(
  uint32_t id,
  rovLight *lights,
  size_t count)
{
  return ROV_Internal::ogl_updateLights(
    &gl_data,
    id,
    lights,
    count
  );
}


// ----------------------------------------------------------- [ Renderpass ] --


void
rov_startRenderPass(
  const rovMat4 view,
  const rovMat4 proj,
  const rovVec3 eye_pos,
  const rovViewport viewport,
  uint32_t clear_flags,
  uint32_t light_buffer)
{
  rov_data.rov_render_passes.emplace_back();
  ROV_Internal::rovRenderPass *rp = &rov_data.rov_render_passes.back();

  rp->light_buffer = light_buffer;

  memcpy(rp->view, view, sizeof(rovMat4));
  memcpy(rp->proj, proj, sizeof(rovMat4));
  memcpy(rp->viewport, viewport, sizeof(rovViewport));
  memcpy(rp->eye_position, eye_pos, sizeof(rovVec3));

  rp->clear_flags = clear_flags;

  rp->materials.emplace_back(rov_curr_material(&rov_data), size_t{0});
}


// ----------------------------------------------------- [ General Settings ] --


void
rov_setColor(const float col[4])
{
  rov_setColor(col[0], col[1], col[2], col[3]);
}


void
rov_setColor(float r, float g, float b, float a)
{
  rov_data.curr_rov_color[0] = r;
  rov_data.curr_rov_color[1] = g;
  rov_data.curr_rov_color[2] = b;
  rov_data.curr_rov_color[3] = a;
}


void
rov_setTexture(uint32_t texture_id, uint32_t texture_slot)
{
  if(texture_slot < ROV_Internal::rov_max_textures)
  {
    rov_data.curr_rov_textures[texture_slot] = texture_id;
  }
}


void
rov_setMesh(uint32_t mesh_id)
{
  rov_data.curr_rov_mesh = mesh_id;
}


void
rov_setShader(uint32_t shader_type)
{
  // -- Param Check -- //
  LIB_ASSERT(shader_type < rovShader_Count);

  rov_data.curr_rov_mesh_shader = shader_type;
}


// ---------------------------------------------------- [ Dbg Line Renderer ] --


void
rov_submitLine(const float start[3], const float end[3])
{
  ROV_Internal::rovLineDrawCall dc;
  memcpy(dc.start, start, sizeof(rovVec3));
  memcpy(dc.end, end, sizeof(rovVec3));
  memcpy(dc.color, rov_data.curr_rov_color, sizeof(rovVec3));

  rov_data.rov_render_passes.back().line_draw_calls.emplace_back(dc);
}


// -------------------------------------------------------- [ Mesh Renderer ] --


void
rov_submitMeshTransform(const float world[16])
{
  ROV_Internal::rovDrawCall dc;
  dc.mesh = rov_data.curr_rov_mesh;
  memcpy(dc.world, world, sizeof(float) * 16);

  size_t draw_calls = 0;

  ROV_Internal::rovMaterial new_mat{};
  new_mat.draw_calls = 1;
  new_mat.material = rov_curr_material(&rov_data);

  const size_t mat_count = rov_data.rov_render_passes.back().materials.size();
  ROV_Internal::rovMaterial *mat = rov_data.rov_render_passes.back().materials.data();
  
  for(size_t i = 0; i < mat_count; ++i)
  {
    // Found a same material
    if(mat[i].material == new_mat.material)
    {
      mat[i].draw_calls += 1;
      rov_data.rov_render_passes.back().draw_calls.insert(draw_calls, dc);
      break;
    }

    // Found insert point.
    if(mat[i].material > new_mat.material)
    {
      rov_data.rov_render_passes.back().materials.insert(i, new_mat);
      rov_data.rov_render_passes.back().draw_calls.insert(draw_calls, dc);
      break;;
    }

    draw_calls += mat[i].draw_calls;
  }

  // Insert into the end if we must
  if(rov_data.rov_render_passes.back().draw_calls.size() == draw_calls)
  {
    rov_data.rov_render_passes.back().materials.emplace_back(new_mat);
    rov_data.rov_render_passes.back().draw_calls.emplace_back(dc);
  }
}
