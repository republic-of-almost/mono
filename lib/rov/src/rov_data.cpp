/*
  We use a generic way of handling the data this makes implimenting the api
  support easier.
*/

#include <rov/rov.hpp>
#include <remotery/Remotery.h>
#include <vector>
#include <string>
#include <stdio.h>
#include <lib/utilities.hpp>
#include <math/math.hpp>
#include <GL/gl3w.h>
#include "rov_platform_exec.hpp"

namespace
{
  std::vector<ROV_Internal::Camera> rov_cameras;

  constexpr uint32_t rov_max_textures = 3;

  float       curr_rov_clear_color[4]{0, 0, 0, 1};
  uint8_t     curr_rov_textures[rov_max_textures]{0};
  uint32_t    curr_rov_mesh = curr_rov_mesh;
  uint8_t     curr_rov_mesh_shader = rovShader_Fullbright;
}


// -- //


namespace
{
  std::vector<ROV_Internal::rovRenderPass> rov_render_passes;


  inline uint64_t
  rov_curr_material()
  {
    /*
      Convert color to uint8_t
    */
    uint8_t red   = (uint8_t)(curr_rov_clear_color[0] * 255);
    uint8_t green = (uint8_t)(curr_rov_clear_color[1] * 255);
    uint8_t blue  = (uint8_t)(curr_rov_clear_color[2] * 255);
    uint8_t alpha = (uint8_t)(curr_rov_clear_color[3] * 255);

    /*
      Pack color together
    */
    const uint32_t color = lib::bits::pack8888(red, green, blue, alpha);


    /*
      Pack in the other details
    */
    uint8_t shader_type = curr_rov_mesh_shader;
    uint8_t texture_01 = curr_rov_textures[0];
    uint8_t texture_02 = curr_rov_textures[1];
    uint8_t texture_03 = curr_rov_textures[2];

    const uint32_t details = lib::bits::pack8888(shader_type, texture_01, texture_02, texture_03);

    return lib::bits::pack3232(details, color);
  }
}


// ----------------------------------------------------------- [ Renderpass ] --


void
rov_startRenderPass(const float view[16], const float proj[16], const uint32_t viewport[4], uint32_t clear_flags)
{
  rov_render_passes.emplace_back();

  memcpy(rov_render_passes.back().view, view, sizeof(float) * 16);
  memcpy(rov_render_passes.back().proj, proj, sizeof(float) * 16);

  memcpy(rov_render_passes.back().viewport, viewport, sizeof(ROV_Internal::rovRenderPass::viewport));

  rov_render_passes.back().clear_flags = clear_flags;

  ROV_Internal::rovMaterial mat{};
  mat.material = rov_curr_material();

  rov_render_passes.back().materials.push_back(mat);
}


// ----------------------------------------------------- [ General Settings ] --


void
rov_setColor(float r, float g, float b, float a)
{
  curr_rov_clear_color[0] = r;
  curr_rov_clear_color[1] = g;
  curr_rov_clear_color[2] = b;
  curr_rov_clear_color[3] = a;
}


void
rov_setTexture(uint32_t texture_id, uint32_t texture_slot)
{
  if(texture_slot < rov_max_textures)
  {
    curr_rov_textures[texture_slot] = texture_id;
  }
}


void
rov_setMesh(uint32_t mesh_id)
{
  curr_rov_mesh = mesh_id;
}


void
rov_setShader(uint32_t shader_type)
{
  curr_rov_mesh_shader = shader_type;
}


// ---------------------------------------------------- [ Dbg Line Renderer ] --


void
rov_submitLine(const float start[3], const float end[3])
{
  ROV_Internal::rovLineDrawCall dc;
  memcpy(dc.start, start, sizeof(ROV_Internal::rovVec3));
  memcpy(dc.end, end, sizeof(ROV_Internal::rovVec3));
  memcpy(dc.color, curr_rov_clear_color, sizeof(ROV_Internal::rovVec3));

  rov_render_passes.back().line_draw_calls.push_back(dc);
}


// -------------------------------------------------------- [ Mesh Renderer ] --


void
rov_submitMeshTransform(const float world[16])
{
  ROV_Internal::rovDrawCall dc;
  dc.mesh = curr_rov_mesh;
  memcpy(dc.world, world, sizeof(float) * 16);

  size_t draw_calls = 0;

  ROV_Internal::rovMaterial new_mat{};
  new_mat.draw_calls = 1;
  new_mat.material = rov_curr_material();

  const size_t mat_count = rov_render_passes.back().materials.size();
  ROV_Internal::rovMaterial *mat = rov_render_passes.back().materials.data();
  
  for(size_t i = 0; i < mat_count; ++i)
  {
    // Found a same material
    if(mat[i].material == new_mat.material)
    {
      mat[i].draw_calls += 1;
      rov_render_passes.back().draw_calls.insert(draw_calls, dc);
      break;
    }

    // Found insert point.
    if(mat[i].material > new_mat.material)
    {
      rov_render_passes.back().materials.insert(i, new_mat);
      rov_render_passes.back().draw_calls.insert(draw_calls, dc);
      break;;
    }

    draw_calls += mat[i].draw_calls;
  }

  // Insert into the end if we must
  if(rov_render_passes.back().draw_calls.size() == draw_calls)
  {
    rov_render_passes.back().materials.emplace_back(new_mat);
    rov_render_passes.back().draw_calls.emplace_back(dc);
  }
}


void
rov_execute()
{
  ROV_Internal::api_execute(rov_render_passes.data(), rov_render_passes.size());

  rov_render_passes.clear();
}
