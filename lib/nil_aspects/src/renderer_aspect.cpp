//#include <GL/gl3w.h>
//
//#define ROV_GL_IMPL
#include <aspect/renderer_aspect.hpp>
#include <rov/rov.hpp>
#include <nil/nil.hpp>
#include <nil/node_event.hpp>
#include <nil/aspect.hpp>
#include <nil/node.hpp>
#include <nil/data/data.hpp>
#include <nil/data/light.hpp>
#include <nil/data/window.hpp>
#include <aspect/math_nil_extensions.hpp>
#include <lib/utilities.hpp>
#include <lib/bench.hpp>


namespace Nil_ext {
namespace ROV_Aspect {


// ----------------------------------------- [ Renderer Aspect Impl Startup ] --


void
start_up(Nil::Engine &engine, Nil::Aspect &aspect)
{
  LOG_INFO("Startup Renderer Aspect")

  Data *self = reinterpret_cast<Data*>(aspect.user_data);
  LIB_ASSERT(self);

  self->current_viewport[0] = 800;
  self->current_viewport[1] = 600;
}


// ------------------------------------------ [ Renderer Aspect Impl Events ] --


void
events(Nil::Engine &engine, Nil::Aspect &aspect, Nil::Event_list &event_list)
{
  Data *self = reinterpret_cast<Data*>(aspect.user_data);
  LIB_ASSERT(self);

  /*
    Updated Window
  */
  {
    size_t count = 0;
    Nil::Data::Window *win;
    
    Nil::Data::events(Nil::Data::Event::UPDATED, &count, &win, nullptr);
    
    if(count)
    {
      /* We operate on 1 window idea so grab the first */
      self->current_viewport[0] = win[0].width;
      self->current_viewport[1] = win[1].height;
    }
  }
  
  // Initialize
  {
    size_t count = 0;
    Nil::Data::Graphics *win;
    
    Nil::Data::events(Nil::Data::Event::ADDED, &count, &win, nullptr);
    
    if(count)
    {
      rov_initialize();
      self->has_initialized = true;
      
      self->light_pack = rov_createLights(nullptr, 0);
    }
  }
  
  
  // Debug Lines
  {
    size_t                count = 0;
    Nil::Data::Developer *data = nullptr;
    Nil::Node            *node = nullptr;
    
    Nil::Data::events(Nil::Data::Event::ADDED, &count, &data, &node);
    
    for(size_t i = 0; i < count; ++i)
    {
      if(data[i].type_id == 2)
      {
        self->debug_lines = node[i];
      }
    }
  }
}


// ------------------------------------- [ Renderer Aspect Impl Early Think ] --


void
early_think(Nil::Engine &engine, Nil::Aspect &aspect)
{
  Data *self = reinterpret_cast<Data*>(aspect.user_data);
  LIB_ASSERT(self);

  BENCH_SCOPED_CPU(ROV_EarlyThink)
  
  if(self->has_initialized)
  {
    /*
      Resources
    */
    {
      size_t                    mesh_rsrc_count = 0;
      Nil::Data::Mesh_resource *mesh_resources = nullptr;
      
      Nil::Data::get(&mesh_rsrc_count, &mesh_resources);
      
      for(size_t i = 0; i < mesh_rsrc_count; ++i)
      {
        Nil::Data::Mesh_resource *mesh_resource = &mesh_resources[i];
        
        if(mesh_resource->status == Nil::Data::Mesh_resource::PENDING)
        {
          const uint32_t mesh = rov_createMesh
          (
            mesh_resource->position_vec3,
            mesh_resource->normal_vec3,
            mesh_resource->texture_coords_vec2,
            mesh_resource->count
          );
          
          if((mesh_resource->id + 1) > self->mesh_ids.size())
          {
            self->mesh_ids.resize(1 << (mesh_resource->id + 1));
          }
          
          self->mesh_ids[mesh_resource->id] = mesh;
          
          mesh_resource->status = Nil::Data::Mesh_resource::LOADED;
        }
      }
      
      size_t                        texture_rsrc_count = 0;
      Nil::Data::Texture_resource   *texture_resources = nullptr;
      
      Nil::Data::get(&texture_rsrc_count, &texture_resources);
      
      for(size_t i = 0; i < texture_rsrc_count; ++i)
      {
        Nil::Data::Texture_resource *texture_resource = &texture_resources[i];
        
        if(texture_resource->status == Nil::Data::Texture_resource::PENDING)
        {
          const uint32_t texture = rov_createTexture(
            texture_resource->data,
            texture_resource->width,
            texture_resource->height,
            texture_resource->sizeof_data,
            texture_resource->compoents == 3 ? rovPixel_RGB8 : rovPixel_RGBA8,
            &texture_resource->platform_resource
          );
          
          if((texture_resource->id + 1) > self->texture_ids.size())
          {
            self->texture_ids.resize(1 << (texture_resource->id + 1));
          }
          
          self->texture_ids[texture_resource->id] = texture;
        }
        
        texture_resource->status = Nil::Data::Texture_resource::LOADED;
      }
    }
    
    /*
      ROV Lighting
    */
    {
      size_t light_count = 0;
      Nil::Data::Light *lights;
      
      Nil::Data::get(&light_count, &lights);
      
      lib::array<rovLight, 32> rov_lights;
      
      rov_lights.clear();
      rov_lights.resize(light_count);
      
      for(size_t i = 0; i < light_count; ++i)
      {
        Nil::Data::Light *light = &lights[i];
      
        if(lights[i].type == Nil::Data::Light::DIR)
        {
          memcpy(rov_lights[i].position, light->direction, sizeof(float) * 3);
          rov_lights[i].position[3] = 1.f;
        }
        else
        {
          memcpy(rov_lights[i].position, light->position, sizeof(float) * 3);
        }
        
        rov_lights[i].color[0] = 1.f;
        rov_lights[i].color[1] = 1.f;
        rov_lights[i].color[2] = 1.f;
        
        rov_lights[i].attenuation[0] = light->atten_const;//0.1f;
        rov_lights[i].attenuation[1] = light->atten_linear;//0.14f;
        rov_lights[i].attenuation[2] = light->atten_exponential;//0.07f;
      }
      
      rov_updateLights(self->light_pack, rov_lights.data(), rov_lights.size());
    }
  }
}


// -------------------------------------- [ Renderer Aspect Impl Late Think ] --


void
think(Nil::Engine &engine, Nil::Aspect &aspect)
{
  Data *self = reinterpret_cast<Data*>(aspect.user_data);
  LIB_ASSERT(self);
  
  BENCH_SCOPED_CPU(ROV_Think)

  if(self->has_initialized)
  {
    size_t cam_count = 0;
    Nil::Data::Camera *cameras;
    Nil::Data::get(&cam_count, &cameras);
    
    for(uint32_t j = 0; j < cam_count; ++j)
    {
      Nil::Data::Camera cam = cameras[j];
      
      uint32_t clear_flags = 0;
      if(cam.clear_color_buffer) { clear_flags |= rovClearFlag_Color; }
      if(cam.clear_depth_buffer) { clear_flags |= rovClearFlag_Depth; }
      
      const uint32_t viewport[4] {
        0,
        0,
        self->current_viewport[0],
        self->current_viewport[1]
      };
      
      const math::mat4 proj = math::mat4_projection(
        cam.width * self->current_viewport[0],
        cam.height * self->current_viewport[1],
        cam.near_plane,
        cam.far_plane,
        cam.fov
      );
      
      rov_startRenderPass(
        cam.view_mat,
        math::mat4_get_data(proj),
        cam.position,
        viewport,
        clear_flags,
        self->light_pack
      );
      
      size_t renderable_count = 0;
      Nil::Data::Renderable *renderables;
      
      Nil::Data::get(&renderable_count, &renderables);
      
      for(size_t i = 0; i < renderable_count; ++i)
      {
        Nil::Data::Renderable render = renderables[i];
      
        if(self->mesh_ids.size() > render.mesh_id)
        {
          rov_setColor(render.color[0], render.color[1], render.color[2], render.color[3]);
          rov_setShader(render.shader);
          rov_setMesh(self->mesh_ids[render.mesh_id]);
          
          if(render.texture_01)
          {
            rov_setTexture(self->texture_ids[render.texture_01], 0);
          }
          
          rov_submitMeshTransform(render.world_mat);
        }
      }
      
      // debug_lines
      if(self->debug_lines)
      {
        Nil::Data::Developer line_data{};
        Nil::Data::get(self->debug_lines, line_data);

        const float *data = (float*)line_data.aux_01;
        size_t count = (size_t)line_data.aux_02;

        LIB_ASSERT(count % 9 == 0);

        const size_t lines = count / 9;

        for(size_t i = 0; i < lines; ++i)
        {
          const size_t index = i * 9;

          rov_setColor(data[index + 6], data[index + 7], data[index + 8], 1.f);
          rov_submitLine(&data[index + 0], &data[index + 3]);
        }

        // Signal to line renderer not reset the data buffer.
        line_data.aux_02 = 0;
        Nil::Data::set(self->debug_lines, line_data);
      }
    }

    rov_execute();
  }
}


} // ns
} // ns
