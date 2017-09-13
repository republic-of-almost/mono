// For now this will live here, it could be its own aspect like GLFW is
#ifndef NVRSUPPORT
#include <GL/gl3w.h>
#include <openvr/openvr.h>
#endif

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
  self->index_ids.emplace_back(uint32_t{0});
  self->texture_ids.emplace_back(uint32_t{0});

  #ifndef NIMGUI
  Nil::Node render_node;
  self->renderer = static_cast<Nil::Node&&>(render_node);
  self->renderer.set_name("ROV");

  Nil::Data::Developer dev{};
  dev.type_id = 1;
  dev.aux_01  = (uintptr_t)ui_menu;
  dev.aux_02  = (uintptr_t)self;
  dev.aux_03  = (uintptr_t)ui_window;
  dev.aux_04  = (uintptr_t)self;

  Nil::Data::set(self->renderer, dev);
  #endif

  #ifndef NVRSUPPORT
  self->vr_view = math::mat4_id();
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

    #ifndef NVRSUPPORT
    if(self->vr_device)
    {
      Nil::Task::cpu_task(
        Nil::Task::CPU::EARLY_THINK,
        (uintptr_t)self,
        update_vr
      );
    }
    #endif
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

  #ifndef NVRSUPPORT
  if(self->vr_device)
  {
    vr::VR_Shutdown();
  }
  #endif
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
          LIB_ASSERT(false);
        }

        // Load up VR GPU resources.
        Nil::Task::gpu_task(
          Nil::Task::GPU::PRE_RENDER,
          (uintptr_t)self,
          load_gpu_vr_resources
        );
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
  LIB_ASSERT(self->vr_device);

  /*
    Create FBOs for the players eyeballs.
  */
  const bool has_left_fbo  = self->eye_render_targets[0] > 0;
  const bool has_right_fbo = self->eye_render_targets[1] > 0;

  LIB_ASSERT(has_left_fbo == has_right_fbo);

  if (!has_left_fbo && !has_right_fbo)
  {
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

      constexpr uint32_t format = rovPixel_RGBA8;

      self->eye_render_targets[0] = rov_createRenderTarget(width, height, format, &eye_data[0].fbo_id);
      self->eye_render_targets[1] = rov_createRenderTarget(width, height, format, &eye_data[1].fbo_id);

      self->eye_platform_ids[0] = eye_data[0].fbo_id;
      self->eye_platform_ids[1] = eye_data[1].fbo_id;

      LIB_ASSERT(self->eye_render_targets[0]);
      LIB_ASSERT(self->eye_render_targets[1]);

      if(self->eye_render_targets[0] && self->eye_render_targets)
      {
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
      else
      {
        LOG_FATAL("VR Resources failed to instatiate");
      }
    }
    else
    {
      LOG_ERROR("VR Render targets are invalid")
    }
  }
  else
  {
    LOG_FATAL("VR Resources not setup correctly")
  }
}

void
update_vr(Nil::Engine &engine, uintptr_t user_data)
{
  BENCH_SCOPED_CPU(ROV_VRUpdate)

  Data *self = reinterpret_cast<Data*>(user_data);
  LIB_ASSERT(self);
  LIB_ASSERT(self->vr_device);

  // -- Vr Events -- //
  {
    vr::VREvent_t event;
    while (self->vr_device->PollNextEvent(&event, sizeof(event)))
    {
      switch (event.eventType)
      {
      case vr::VREvent_TrackedDeviceActivated:
      {
        //SetupRenderModelForTrackedDevice(event.trackedDeviceIndex);
        printf("Device %u attached. Setting up render model.\n", event.trackedDeviceIndex);
      }
      break;
      case vr::VREvent_TrackedDeviceDeactivated:
      {
        printf("Device %u detached.\n", event.trackedDeviceIndex);
      }
      break;
      case vr::VREvent_TrackedDeviceUpdated:
      {
        printf("Device %u updated.\n", event.trackedDeviceIndex);
      }
      break;
      }
    }

    // Process SteamVR controller state
    for (vr::TrackedDeviceIndex_t unDevice = 0; unDevice < vr::k_unMaxTrackedDeviceCount; unDevice++)
    {
      vr::VRControllerState_t state;
      if (self->vr_device->GetControllerState(unDevice, &state, sizeof(state)))
      {
        //m_rbShowTrackedDevice[unDevice] = state.ulButtonPressed == 0;
      }
    }
  }

  // -- Vr Update -- //
  {
    vr::VRCompositor()->WaitGetPoses(self->tracked_device_pose, vr::k_unMaxTrackedDeviceCount, NULL, 0);

    self->valid_pose_count = 0;
    self->pose_classes.clear();

    for (int nDevice = 0; nDevice < vr::k_unMaxTrackedDeviceCount; ++nDevice)
    {
      if (self->tracked_device_pose[nDevice].bPoseIsValid)
      {
        self->valid_pose_count++;

        auto m = self->tracked_device_pose[nDevice].mDeviceToAbsoluteTracking;
        const float mat[]
        {
          m.m[0][0], m.m[1][0], m.m[2][0], 0.0,
          m.m[0][1], m.m[1][1], m.m[2][1], 0.0,
          m.m[0][2], m.m[1][2], m.m[2][2], 0.0,
          m.m[0][3], m.m[1][3], m.m[2][3], 1.0f
        };
          
        self->device_pose[nDevice] = math::mat4_init(mat);

        if (self->device_char[nDevice] == 0)
        {
          switch (self->vr_device->GetTrackedDeviceClass(nDevice))
          {
          case vr::TrackedDeviceClass_Controller:        self->device_char[nDevice] = 'C'; break;
          case vr::TrackedDeviceClass_HMD:               self->device_char[nDevice] = 'H'; break;
          case vr::TrackedDeviceClass_Invalid:           self->device_char[nDevice] = 'I'; break;
          case vr::TrackedDeviceClass_GenericTracker:    self->device_char[nDevice] = 'G'; break;
          case vr::TrackedDeviceClass_TrackingReference: self->device_char[nDevice] = 'T'; break;
          default:                                       self->device_char[nDevice] = '?'; break;
          }
        }
        self->pose_classes.emplace_back(self->device_char[nDevice]);
      }
    }

    if (self->tracked_device_pose[vr::k_unTrackedDeviceIndex_Hmd].bPoseIsValid)
    {
      self->vr_view = self->device_pose[vr::k_unTrackedDeviceIndex_Hmd];
      self->vr_view = math::mat4_get_inverse(self->vr_view);
    }
  }

  //vr::HmdMatrix34_t matEyeRight = self->vr_device->GetEyeToHeadTransform(vr::Eye_Left);
  //
  //const float data[]{
  //  matEyeRight.m[0][0], matEyeRight.m[1][0], matEyeRight.m[2][0], 0.0,
  //  matEyeRight.m[0][1], matEyeRight.m[1][1], matEyeRight.m[2][1], 0.0,
  //  matEyeRight.m[0][2], matEyeRight.m[1][2], matEyeRight.m[2][2], 0.0,
  //  matEyeRight.m[0][3], matEyeRight.m[1][3], matEyeRight.m[2][3], 1.0f
  //};

  //self->vr_view = math::mat4_init(data);
  //self->vr_view = math::mat4_get_inverse(self->vr_view);
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
        const bool data_is_array    = tex->data_type == Nil::Resource::Texture::DATA;

        // -- //
        
        int x = 0;
        int y = 0;
        int c = 0;
        stbi_uc *img_data = nullptr;

        if (data_is_filename)
        {
          const char *path = (const char*)tex->data;
          img_data = stbi_load(path, &x, &y, &c, 0);

        }
        else if(data_is_array)
        {
          const stbi_uc *data = (stbi_uc*)tex->data;
          img_data = stbi_load_from_memory(data, tex->data_size, &x, &y, &c, 0);
        }
        else
        {
          LIB_ASSERT(false);
          LOG_ERROR("Unknown data type to load texture");
          tex->status = Nil::Resource::Texture::FAILED;
          continue;
        }
        
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
      else if (!has_data && i > 0)
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
        
        // Index is optional
        uint32_t index = 0;
        if(mesh_resource->index_count)
        {
          index = rov_createIndex(mesh_resource->index, mesh_resource->index_count);
        }
        
        self->index_ids.emplace_back(index);
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
  self->index_ids.clear();
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
    math::mat4 proj_mat = math::mat4_perspective(
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

      rov_lights[i].color[0] = (float)light->color[0] / 255.f;
      rov_lights[i].color[1] = (float)light->color[1] / 255.f;;
      rov_lights[i].color[2] = (float)light->color[2] / 255.f;;

      rov_lights[i].attenuation[0] = light->atten_const;
      rov_lights[i].attenuation[1] = light->atten_linear;
      rov_lights[i].attenuation[2] = light->atten_exponential;
    }

    rov_updateLights(self->light_pack, rov_lights.data(), rov_lights.size());
  }
}


/*
  Convert Nil Data into ROV Data
*/
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

    const math::mat4 cam_proj = math::mat4_perspective(
      cam.width * self->current_viewport[0],
      cam.height * self->current_viewport[1],
      cam.near_plane,
      cam.far_plane,
      cam.fov
    );

    // Rendertargets //
    struct Render_target_settings
    {
      uint32_t render_target;
      float view[16];
      float proj[16];
    };

    #ifndef NVRSUPPORT
    Render_target_settings targets[]{
      {0, {}, {}},
      {self->eye_render_targets[0], {}, {}},
      {self->eye_render_targets[1], {}, {}},
     };

     // Normal View
     memcpy(targets[0].view, cam.view_mat, sizeof(Render_target_settings::view));
     memcpy(targets[0].proj, cam_proj.data, sizeof(Render_target_settings::proj));

     math::mat4 cam_view = math::mat4_init(targets[0].view);

     // Left Eye
     {
       math::mat4 final_view = math::mat4_multiply(cam_view, self->vr_view);

       memcpy(targets[1].view, final_view.data, sizeof(Render_target_settings::view));
       const vr::HmdMatrix44_t left_proj = self->vr_device->GetProjectionMatrix(vr::Eye_Left, 0.1f, 300.f);

       const float left_proj_mat[] {
         left_proj.m[0][0], left_proj.m[1][0], left_proj.m[2][0], left_proj.m[3][0],
         left_proj.m[0][1], left_proj.m[1][1], left_proj.m[2][1], left_proj.m[3][1],
         left_proj.m[0][2], left_proj.m[1][2], left_proj.m[2][2], left_proj.m[3][2],
         left_proj.m[0][3], left_proj.m[1][3], left_proj.m[2][3], left_proj.m[3][3]
       };

       memcpy(targets[1].proj, left_proj_mat, sizeof(Render_target_settings::proj));
     }

     // Right Eye
     {
       math::mat4 final_view = math::mat4_multiply(cam_view, self->vr_view);

       memcpy(targets[2].view, final_view.data, sizeof(Render_target_settings::view));
       const vr::HmdMatrix44_t right_proj = self->vr_device->GetProjectionMatrix(vr::Eye_Right, 0.1f, 300.f);

       const float right_proj_mat[]{
         right_proj.m[0][0], right_proj.m[1][0], right_proj.m[2][0], right_proj.m[3][0],
         right_proj.m[0][1], right_proj.m[1][1], right_proj.m[2][1], right_proj.m[3][1],
         right_proj.m[0][2], right_proj.m[1][2], right_proj.m[2][2], right_proj.m[3][2],
         right_proj.m[0][3], right_proj.m[1][3], right_proj.m[2][3], right_proj.m[3][3]
       };
     
       memcpy(targets[2].proj, right_proj_mat, sizeof(Render_target_settings::proj));
     }

    #else
    Render_target_settings targets[] {
      {0, {}},
    };

    // Normal View
    memcpy(targets[0].view, cam.view_mat, sizeof(Render_target_settings::view));
    memcpy(targets[0].proj, cam_proj.data, sizeof(Render_target_settings::proj));

    math::mat4 cam_view = math::mat4_init(targets[0].view);
    #endif

    for(const Render_target_settings &rt : targets)
    {
      const uint32_t viewport[4] {
        0,
        0,
        self->current_viewport[0],
        self->current_viewport[1]
      };

      rov_setColor(cam.clear_color);
      
      if(self->show_wireframe_mode)
      {
        rov_setRasterizer(rovRasterizer{true});
      }
      else
      {
        rov_setRasterizer(rovRasterizer{false});
      }

      rov_startRenderPass(
        rt.view,
        rt.proj,
        cam.position,
        viewport,
        clear_flags,
        self->light_pack,
        rt.render_target
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
          
          const uint32_t index_id = self->index_ids[render.mesh_id];
          rov_setIndex(index_id);

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
          else
          {
            rov_setTexture(0, 0);
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

  // Get ROV To Render
  // Move this to a GPU Task called render.
  rov_execute();

  #ifndef NVRSUPPORT
  /*
    Need to pass the FBO's to the VR headset.
  */
  if (self->vr_device)
  {
    // left
    vr::Texture_t left_eye{
      (void*)(uintptr_t)self->eye_platform_ids[0],
      vr::TextureType_OpenGL,
      vr::ColorSpace_Gamma
    };

    vr::VRCompositor()->Submit(vr::Eye_Left, &left_eye);

    // right
    vr::Texture_t right_eye{
      (void*)(uintptr_t)self->eye_platform_ids[1],
      vr::TextureType_OpenGL,
      vr::ColorSpace_Gamma
    };

    vr::VRCompositor()->Submit(vr::Eye_Right, &right_eye);
  }

  glFlush();
  glFinish();
  #endif 
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

  if(ImGui::BeginMenu("Renderer"))
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
      ImGui::Checkbox("Enable Debug Lines",           &self->show_debug_lines);
      ImGui::Checkbox("Enable Debug Bounding Boxes",  &self->show_debug_bounding_boxes);
      ImGui::Checkbox("Enable Lookat Bounding Box",   &self->show_lookat_bounding_box);
      ImGui::Checkbox("Enable Lookat Indicator",      &self->show_lookat_cross);
      #else
      bool no_debug_line = false;
      ImGui::Text("Some options disabled - Require debug lines to be enabled");
      ImGui::Checkbox("Enable Debug Lines",           &no_debug_line);
      ImGui::Checkbox("Enable Debug Bounding Boxes",  &no_debug_line);
      ImGui::Checkbox("Enable Lookat Bounding Box",   &no_debug_line);
      ImGui::Checkbox("Enable Lookat Indicator",      &no_debug_line);
      #endif
      ImGui::Checkbox("Enable Wireframe Mode",        &self->show_wireframe_mode);
      
      ImGui::End();
    }
    
  }

  #endif
}



} // ns
} // ns
