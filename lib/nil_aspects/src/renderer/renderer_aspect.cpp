#include <aspect/renderer_aspect.hpp>
#include <renderer/rov_extensions.hpp>
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

#ifndef NVRSUPPORT
#include <openvr.h>
#endif


namespace Nil_ext {
namespace ROV_Aspect {


// ---------------------------------------------------- [ Renderer Lifetime ] --


void
start_up(Nil::Engine &engine, Nil::Aspect &aspect)
{
  LOG_INFO("Startup Renderer Aspect")

  Data *self = reinterpret_cast<Data*>(aspect.user_data);
  LIB_ASSERT(self);

  self->mesh_ids.emplace_back(uint32_t{0});
  self->texture_ids.emplace_back(uint32_t{0});

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
        Nil::Task::cpu_task(Nil::Task::CPU::EARLY_THINK, (uintptr_t)self, initialize_rov);
      }

      // Added Debug Lines
      #ifndef NDEBUGLINES
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
      #endif
    }
  }

  
  if(self->has_initialized)
  {
    Nil::Task::gpu_task(
      Nil::Task::GPU::PRE_RENDER,
      (uintptr_t)self,
      load_gpu_resources
    );

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
  
  #ifndef NDEBUGLINES
  if(self->show_lookat_bounding_box)
  {
    Nil::Task::cpu_task(
      Nil::Task::CPU::EARLY_THINK,
      (uintptr_t)self,
      find_lookat_bounding_box
    );
  }
  #endif
}


void
shut_down(Nil::Engine &engine, Nil::Aspect &aspect)
{
  Data *self = reinterpret_cast<Data*>(aspect.user_data);
  LIB_ASSERT(self);

  if(self->has_initialized)
  {
    Nil::Task::gpu_task(
      Nil::Task::GPU::PRE_RENDER,
      (uintptr_t)self,
      unload_gpu_resources
    );
  }
}


// ------------------------------------------------------- [ Renderer Tasks ] --


void
initialize_rov(Nil::Engine &engine, uintptr_t user_data)
{
  BENCH_SCOPED_CPU(ROV_InitializeROV)

  Data *self = reinterpret_cast<Data*>(user_data);
  LIB_ASSERT(self);


  size_t count = 0;
  Nil::Data::Window *win = nullptr;
  Nil::Data::get(&count, &win);

  if (count)
  {
    /* We operate on 1 window idea so grab the first */
    self->current_viewport[0] = win[0].width;
    self->current_viewport[1] = win[0].height;

    if (win[0].type == Nil::Data::Window::OGL)
    {
      // -- ROV -- //

      LOG_INFO("Initialize ROV")

      rov_initialize(Nil::Resource::asset_path());

      self->has_initialized = true;
      self->light_pack = rov_createLights(nullptr, 0);

      // -- Init the VR Device -- //

      #ifndef NVRSUPPORT
      vr::EVRInitError vr_error = vr::VRInitError_None;
      self->vr_device = vr::VR_Init(&vr_error, vr::VRApplication_Scene);

      if(vr_error != vr::VRInitError_None)
      {
        char buf[1024]{};
        sprintf(buf, "Unable to init VR runtime: %s", vr::VR_GetVRInitErrorAsEnglishDescription(vr_error));

        LOG_ERROR(buf);
      }

      if (self->vr_device)
      {
        char t_system[2048]{};
        uint32_t length = self->vr_device->GetStringTrackedDeviceProperty(vr::k_unTrackedDeviceIndex_Hmd, vr::Prop_TrackingSystemName_String, nullptr, 0, nullptr);
        self->vr_device->GetStringTrackedDeviceProperty(vr::k_unTrackedDeviceIndex_Hmd, vr::Prop_TrackingSystemName_String, t_system, 2048, nullptr);

        LOG_INFO(t_system);

        if (!vr::VRCompositor())
        {
          assert(false);
        }

        // Load up VR GPU resources.
        Nil::Task::gpu_task(Nil::Task::GPU::PRE_RENDER, (uintptr_t)self, load_gpu_vr_resources);
      }
      #endif
    }
  }
}


#ifndef NVRSUPPORT
void
load_gpu_vr_resources(Nil::Engine &engine, uintptr_t user_data)
{
  BENCH_SCOPED_CPU(ROV_LoadVRGPUResources)

  Data *self = reinterpret_cast<Data*>(user_data);
  LIB_ASSERT(self);

  const bool has_left_fbo  = self->eye_render_targets[0] > 0;
  const bool has_right_fbo = self->eye_render_targets[1] > 0;

  LIB_ASSERT(has_left_fbo == has_right_fbo);

  if (!has_left_fbo && !has_right_fbo)
  {
    LIB_ASSERT(self->vr_device);

    uint32_t width  = 0;
    uint32_t height = 0;

    self->vr_device->GetRecommendedRenderTargetSize(&width, &height);

    LIB_ASSERT(width);
    LIB_ASSERT(height);

    if(width && height)
    {
      struct Eye_fbo
      {
        uintptr_t fbo_id;
        const char *name;
      };

      Eye_fbo eye_data[]
      {
        {0, "VR Left Eye"},
        {0, "VR Right Eye"},
      };

      self->eye_render_targets[0] = rov_createRenderTarget(width, height, rovPixel_RGBA8, &eye_data[0].fbo_id);
      self->eye_render_targets[1] = rov_createRenderTarget(width, height, rovPixel_RGBA8, &eye_data[1].fbo_id);

      for(auto &e : eye_data)
      {
        Nil::Resource::Texture tex{};
        tex.name              = e.name;
        tex.width             = width;
        tex.height            = height;
        tex.platform_resource = e.fbo_id;
        tex.data_type         = Nil::Resource::Texture::LOCAL;
        tex.status            = Nil::Resource::Texture::LOADED;

        Nil::Resource::load(tex);
      }
    }
  }
}
#endif


void
load_gpu_resources(Nil::Engine &engine, uintptr_t user_data)
{
  BENCH_SCOPED_CPU(ROV_LoadGPUResources)

  Data *self = reinterpret_cast<Data*>(user_data);
  LIB_ASSERT(self);

  /*
    Load textures
  */
  {
    size_t count = 0;
    Nil::Resource::Texture *textures = nullptr;

    Nil::Resource::get(&count, &textures);

    for (size_t i = 0; i < count; ++i)
    {
      Nil::Resource::Texture *tex = &textures[i];

      const bool has_data = tex && !!tex->data;
      const bool is_pending = tex && tex->status == Nil::Resource::Texture::PENDING;

      if (has_data && is_pending)
      {
        const bool data_is_filename = tex->data_type == Nil::Resource::Texture::FILENAME;

        if (data_is_filename)
        {
          int x = 0;
          int y = 0;
          int c = 0;

          stbi_uc *img_data = nullptr;
          const char *path = (const char*)tex->data;

          stbi_set_flip_vertically_on_load(true);
          img_data = stbi_load(path, &x, &y, &c, 0);

          // -- Did we Fail to load texture -- //
          if (img_data == nullptr)
          {
            tex->status = Nil::Resource::Texture::FAILED;

            char err_msg[1024]{};
            strcat(err_msg, "Failed to load texture: ");

            LOG_ERROR(err_msg);
            LIB_ASSERT(false);
            continue;
          }

          const uint32_t format = c == 3 ? rovPixel_RGB8 : rovPixel_RGBA8;

          const uint32_t tex_id = rov_createTexture(
            img_data,
            x,
            y,
            x * y * c,
            format,
            &tex->platform_resource
          );

          tex->width = x;
          tex->height = y;
          tex->components = c;

          if ((tex->id) >= self->texture_ids.size())
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
      else if (!has_data)
      {
        LIB_ASSERT(false);
        LOG_ERROR("Tried to load a texture with no data");
        tex->status = Nil::Resource::Texture::FAILED;
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

    for (size_t i = 0; i < count; ++i)
    {
      Nil::Resource::Mesh *mesh_resource = &meshes[i];

      if (mesh_resource->status == Nil::Resource::Mesh::PENDING)
      {
        if (mesh_resource->triangle_count == 0)
        {
          mesh_resource->status = Nil::Resource::Mesh::LOADED;
          continue;
        }

        const uint32_t mesh = rov_createMesh
        (
          mesh_resource->position_vec3,
          mesh_resource->normal_vec3,
          mesh_resource->texture_coords_vec2,
          mesh_resource->triangle_count,
          &mesh_resource->platform_resource
        );

        self->mesh_ids.emplace_back(mesh);

        mesh_resource->status = Nil::Resource::Mesh::LOADED;
      }
    }
  } // Load Meshes
}


void
unload_gpu_resources(Nil::Engine &engine, uintptr_t user_data)
{
  BENCH_SCOPED_CPU(ROV_UnloadResources);
  
  Data *self = reinterpret_cast<Data*>(user_data);
  LIB_ASSERT(self);

  rov_destroy();

  /*
    This invalidates all the resources we have so clear them.
  */
  self->texture_ids.clear();
  self->mesh_ids.clear();
}


#ifndef NDEBUGLINES
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
#endif


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

    // Rendertargets //
    #ifndef NVRSUPPORT
    const uint32_t targets[]{
      0,
      self->eye_render_targets[0],
      self->eye_render_targets[1],
     };
    #else
    const uint32_t targets[] {0};
    #endif

    for(const uint32_t rt_id : targets)
    {
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
        self->light_pack,
        rt_id
      );

      size_t renderable_count = 0;
      Nil::Data::Renderable *renderables;

      Nil::Data::get(&renderable_count, &renderables);

      for(size_t i = 0; i < renderable_count; ++i)
      {
        Nil::Data::Renderable render = renderables[i];

        const uint32_t mesh_count = self->mesh_ids.size();

        if(render.mesh_id && mesh_count > render.mesh_id)
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
            else
            {
              LOG_WARNING_ONCE("Failed to find texture");
            }
          }

          rov_submitMeshTransform(render.world_mat);
        }
      }

      // debug_lines
      #ifndef NDEBUGLINES
      {
        Nil::Data::Developer line_data{};
      
        if(Nil::Data::has(self->debug_lines, line_data))
        {
          Nil::Data::get(self->debug_lines, line_data);

          const float *data = (float*)line_data.aux_01;
          size_t count = (size_t)line_data.aux_02;

          if (self->debug_lines && self->show_debug_lines)
          {
            LIB_ASSERT(count % 9 == 0);

            const size_t lines = count / 9;

            for(size_t i = 0; i < lines; ++i)
            {
              const size_t index = i * 9;

              rov_setColor(data[index + 6], data[index + 7], data[index + 8], 1.f);
              rov_submitLine(&data[index + 0], &data[index + 3]);
            }
          }

          // Signal to line renderer not reset the data buffer.
          line_data.aux_02 = 0;
          Nil::Data::set(self->debug_lines, line_data);
        }
      }
    
      // Lookat boxes
      if(self->show_lookat_bounding_box)
      {
        size_t count = self->selected_bbs.size();
        Nil::Data::Bounding_box *data = self->selected_bbs.data();
      
        rov_setColor(1, 0, 1, 1);
      
        Nil_ext::rov_render_bounding_box(data, count);
      }

      // Bounding boxes
      if(self->show_debug_bounding_boxes)
      {
        size_t count = 0;
        Nil::Data::Bounding_box *data = nullptr;
        Nil::Data::get(&count, &data, true);

        rov_setColor(0, 1, 0, 1);
      
        Nil_ext::rov_render_bounding_box(data, count);
      }
    
      // Look at marker
      if(self->show_lookat_cross)
      {
        Nil_ext::rov_render_camera_cross(cam, self->current_viewport);
      }
    }
    #endif
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
      #ifndef NDEBUGLINES
      ImGui::Text("Debug Line Options");
      ImGui::Checkbox("Enable Debug Lines", &self->show_debug_lines);
      ImGui::Checkbox("Enable Debug Bounding Boxes", &self->show_debug_bounding_boxes);
      ImGui::Checkbox("Enable Lookat Bounding Box", &self->show_lookat_bounding_box);
      ImGui::Checkbox("Enable Lookat Indicator", &self->show_lookat_cross);
      #else
      ImGui::Text("No debug line support, rebuild without NDEBUGLINES enabled");
      #endif
      
      ImGui::End();
    }
    
  }

  #endif
}



} // ns
} // ns
