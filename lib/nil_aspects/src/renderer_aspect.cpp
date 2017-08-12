#include <aspect/renderer_aspect.hpp>
#include <nil/nil.hpp>
#include <nil/aspect.hpp>
#include <nil/node.hpp>
#include <nil/data/data.hpp>
#include <nil/resource/resource.hpp>
#include <rov/rov.hpp>
#include <lib/utilities.hpp>
#include <stb/stb_image.h>
#include <math/math.hpp>

#ifndef NIMGUI
#include <imgui/imgui.h>
#endif


namespace Nil_ext {
namespace ROV_Aspect {


// ----------------------------------------- [ Renderer Aspect Impl Startup ] --


void
start_up(Nil::Engine &engine, Nil::Aspect &aspect)
{
  LOG_INFO("Startup Renderer Aspect")

  Data *self = reinterpret_cast<Data*>(aspect.user_data);
  LIB_ASSERT(self);

  self->mesh_ids.emplace_back(uint32_t{0});

  #ifndef NIMGUI
  Nil::Node render_node;
  self->renderer = static_cast<Nil::Node&&>(render_node);
  self->renderer.set_name("ROV");

  Nil::Data::Developer dev{};
  dev.type_id = 1;
  dev.aux_01  = (uintptr_t)ui_menu;
  dev.aux_02 = (uintptr_t)self;
  dev.aux_03 = (uintptr_t)ui_window;
  dev.aux_04 = (uintptr_t)self;

  Nil::Data::set(self->renderer, dev);
  #endif
}


// ------------------------------------------ [ Renderer Aspect Impl Events ] --


void
events(Nil::Engine &engine, Nil::Aspect &aspect)
{
  Data *self = reinterpret_cast<Data*>(aspect.user_data);
  LIB_ASSERT(self);

  /*
    Updated Window
  */
  {
    size_t count = 0;
    Nil::Data::Window *win = nullptr;
    Nil::Data::get(&count, &win);

    if(count)
    {
      /* We operate on 1 window idea so grab the first */
      self->current_viewport[0] = win[0].width;
      self->current_viewport[1] = win[0].height;

      if(!self->has_initialized)
      {
        if(win[0].type == Nil::Data::Window::OGL)
        {
          LOG_INFO("Initialize ROV")

          rov_initialize(Nil::Resource::asset_path());
          self->has_initialized = true;

          self->light_pack = rov_createLights(nullptr, 0);
        }
      }

      // Added Debug Lines
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
  }

  /*
    Load textures
  */
  {
    size_t count = 0;
    Nil::Resource::Texture *textures = nullptr;

    Nil::Resource::get(&count, &textures);

    for(size_t i = 0; i < count; ++i)
    {
      Nil::Resource::Texture *tex = &textures[i];

      if(tex->status == Nil::Resource::Texture::PENDING)
      {
        if(tex->data_type == Nil::Resource::Texture::FILENAME)
        {
          int x = 0;
          int y = 0;
          int c = 0;
          stbi_uc *img_data = nullptr;
          const char *path = (const char*)tex->data;

          stbi_set_flip_vertically_on_load(true);
          img_data = stbi_load(path, &x, &y, &c, 0);

          const uint32_t format = c == 3 ? rovPixel_RGB8 : rovPixel_RGBA8;

          const uint32_t tex_id = rov_createTexture(
            img_data,
            x,
            y,
            x * y * c,
            format,
            &tex->platform_resource
          );

          tex->id         = tex_id;
          tex->width      = x;
          tex->height     = y;
          tex->components = c;

          if((tex->id) > self->texture_ids.size())
          {
            const size_t new_size = (tex->id + 1);
            self->texture_ids.resize(new_size);
          }

          const size_t id = tex->id;
          self->texture_ids[id] = tex_id;

          stbi_image_free(img_data);

          tex->status = Nil::Resource::Texture::LOADED;
        }
      }
    }
  } // Load Textures

  /*
    Load Meshes
  */
  {
    size_t count = 0;
    Nil::Resource::Mesh *meshes = nullptr;

    Nil::Resource::get(&count, &meshes);

    for(size_t i = 0; i < count; ++i)
    {
      Nil::Resource::Mesh *mesh_resource = &meshes[i];

      if(mesh_resource->status == Nil::Resource::Mesh::PENDING)
      {
        if(mesh_resource->count == 0)
        {
          mesh_resource->status = Nil::Resource::Mesh::LOADED;
          continue;
        }

        const uint32_t mesh = rov_createMesh
        (
          mesh_resource->position_vec3,
          mesh_resource->normal_vec3,
          mesh_resource->texture_coords_vec2,
          mesh_resource->count,
          &mesh_resource->platform_resource
        );

        self->mesh_ids.emplace_back(mesh);

        mesh_resource->status = Nil::Resource::Mesh::LOADED;
      }
    }
  } // Load Meshes
  
  if(self->has_initialized)
  {
    Nil::Task::cpu_task(
      Nil::Task::CPU::EARLY_THINK,
      (uintptr_t)self,
      early_think
    );
    
    Nil::Task::cpu_task(
      Nil::Task::CPU::THINK,
      (uintptr_t)self,
      think
    );
  }
  
  if(self->show_lookat_bounding_box)
  {
    Nil::Task::cpu_task(
      Nil::Task::CPU::EARLY_THINK,
      (uintptr_t)self,
      find_lookat_bounding_box
    );
  }
}


// ------------------------------------------------------- [ Renderer Tasks ] --


void
find_lookat_bounding_box(Nil::Engine &engine, uintptr_t user_data)
{
  BENCH_SCOPED_CPU(ROV_FindLookAtBoundingBox)
  
  Data *self = reinterpret_cast<Data*>(user_data);
  LIB_ASSERT(self);
  
  size_t cam_count = 0;
  Nil::Data::Camera *cameras;
  Nil::Data::get(&cam_count, &cameras);
  
  size_t bb_count = 0;
  Nil::Data::Bounding_box *boxes = nullptr;
  Nil::Data::get(&bb_count, &boxes, true);
  
  self->selected_bbs.clear();
  
  for(size_t i = 0; i < cam_count; i++)
  {
  
    // Generate ray.
    math::mat4 proj_mat = math::mat4_projection(
      cameras[i].width * self->current_viewport[0],
      cameras[i].height * self->current_viewport[1],
      cameras[i].near_plane,
      cameras[i].far_plane,
      cameras[i].fov
    );
    
    math::mat4 view_mat = math::mat4_init(cameras[i].view_mat);
    
    math::ray r = math::ray_from_perpective_viewport(
      cameras[i].width * self->current_viewport[0],
      cameras[i].height * self->current_viewport[1],
      self->current_viewport[0] / 2,
      self->current_viewport[1] / 2,
      proj_mat,
      view_mat,
      math::vec3_init(cameras[i].position),
      100000
    );
  
    for(size_t j = 0; j < bb_count; j++)
    {
      math::aabb box = math::aabb_init(boxes[j].min, boxes[j].max);
      
      float distance = math::ray_test_aabb(r, box);
    
      if(distance > 0.f)
      {
        self->selected_bbs.emplace_back(boxes[j]);
      }
    }
  }
}


void
early_think(Nil::Engine &engine, uintptr_t user_data)
{
  BENCH_SCOPED_CPU(ROV_EarlyThink)

  Data *self = reinterpret_cast<Data*>(user_data);
  LIB_ASSERT(self);

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

      rov_lights[i].attenuation[0] = light->atten_const;
      rov_lights[i].attenuation[1] = light->atten_linear;
      rov_lights[i].attenuation[2] = light->atten_exponential;
    }

    rov_updateLights(self->light_pack, rov_lights.data(), rov_lights.size());
  }
}


// -------------------------------------- [ Renderer Aspect Impl Late Think ] --


void
think(Nil::Engine &engine, uintptr_t user_data)
{
  BENCH_SCOPED_CPU(ROV_Think)

  Data *self = reinterpret_cast<Data*>(user_data);
  LIB_ASSERT(self);

  size_t cam_count = 0;
  Nil::Data::Camera *cameras;
  Nil::Data::get(&cam_count, &cameras);

  size_t mat_count = 0;
  Nil::Resource::Material *mats;
  Nil::Resource::get(&mat_count, &mats);

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

    rov_setColor(cam.clear_color);

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

      const uint32_t mesh_count = self->mesh_ids.size();

      if(mesh_count > render.mesh_id)
      {
        const Nil::Resource::Material mat = mats[render.material_id];

        const float colorf[4]
        {
          lib::color::get_channel_1f(mat.color),
          lib::color::get_channel_2f(mat.color),
          lib::color::get_channel_3f(mat.color),
          lib::color::get_channel_4f(mat.color),
        };

        rov_setColor(colorf);

        const uint32_t mesh_id = self->mesh_ids[render.mesh_id];
        rov_setMesh(mesh_id);

        const uint32_t texture_01 = mats[render.material_id].texture_01;

        if(texture_01)
        {
          const uint32_t texture_count = self->texture_ids.size();

          if(texture_count > texture_01)
          {
            const uint32_t texture_id = self->texture_ids[texture_01];
            rov_setTexture(texture_id, 0);
          }
        }

        rov_submitMeshTransform(render.world_mat);
      }
    }

    // debug_lines
    if(self->debug_lines && self->show_debug_lines)
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

    // Bounding boxes
    if(self->show_debug_bounding_boxes)
    {
//      size_t count = 0;
//      Nil::Data::Bounding_box *data = nullptr;
//      Nil::Data::get(&count, &data, true);

      size_t count = self->selected_bbs.size();
      Nil::Data::Bounding_box *data = self->selected_bbs.data();

      rov_setColor(0, 1, 0, 1);

      for(size_t i = 0; i < count; ++i)
      {
        const float *min = data[i].min;
        const float *max = data[i].max;
      
        // Y lines
        {
          const float a[3] { min[0], min[1], min[2] };
          const float b[3] { min[0], max[1], min[2] };
          rov_submitLine(a, b);

          const float c[3] { max[0], min[1], min[2] };
          const float d[3] { max[0], max[1], min[2] };
          rov_submitLine(c, d);

          const float e[3] { min[0], min[1], max[2] };
          const float f[3] { min[0], max[1], max[2] };
          rov_submitLine(e, f);

          const float g[3] { max[0], min[1], max[2] };
          const float h[3] { max[0], max[1], max[2] };
          rov_submitLine(g, h);
        }

        // X Lines
        {
          const float a[3] { min[0], min[1], min[2] };
          const float b[3] { max[0], min[1], min[2] };
          rov_submitLine(a, b);

          const float c[3] { min[0], max[1], min[2] };
          const float d[3] { max[0], max[1], min[2] };
          rov_submitLine(c, d);

          const float e[3] { min[0], min[1], max[2] };
          const float f[3] { max[0], min[1], max[2] };
          rov_submitLine(e, f);

          const float g[3] { min[0], max[1], max[2] };
          const float h[3] { max[0], max[1], max[2] };
          rov_submitLine(g, h);
        }

        // Z lines
        {
          const float a[3] { min[0], min[1], min[2] };
          const float b[3] { min[0], min[1], max[2] };
          rov_submitLine(a, b);

          const float c[3] { max[0], min[1], min[2] };
          const float d[3] { max[0], min[1], max[2] };
          rov_submitLine(c, d);

          const float e[3] { min[0], max[1], min[2] };
          const float f[3] { min[0], max[1], max[2] };
          rov_submitLine(e, f);

          const float g[3] { max[0], max[1], min[2] };
          const float h[3] { max[0], max[1], max[2] };
          rov_submitLine(g, h);
        }
      }
    }
  }

  rov_execute();
}


// ------------------------------------------------------------------- [ UI ] --


void
ui_menu(uintptr_t user_data)
{
  #ifndef NIMGUI
  Nil_ext::ROV_Aspect::Data *self(
    reinterpret_cast<Nil_ext::ROV_Aspect::Data*>(user_data)
  );

  LIB_ASSERT(self);

  if(ImGui::BeginMenu("ROV"))
  {
    ImGui::MenuItem("Show Debug Options", nullptr, &self->show_debug_options);

    ImGui::EndMenu();
  }
  #endif
}


void
ui_window(uintptr_t user_data)
{
  #ifndef NIMGUI
  Nil_ext::ROV_Aspect::Data *self(
    reinterpret_cast<Nil_ext::ROV_Aspect::Data*>(user_data)
  );

  LIB_ASSERT(self);
  
  if(self->show_debug_options)
  {
    if(ImGui::Begin("Debug Options", &self->show_debug_options))
    {
      ImGui::Text("Debug Line Options");
      ImGui::Checkbox("Enable Debug Lines", &self->show_debug_lines);
      ImGui::Checkbox("Enable Debug Bounding Boxes", &self->show_debug_bounding_boxes);
      ImGui::Checkbox("Enable Lookat Bounding Box", &self->show_lookat_bounding_box);
      
      ImGui::End();
    }
    
  }

  #endif
}



} // ns
} // ns
