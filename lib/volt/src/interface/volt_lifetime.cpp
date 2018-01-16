#include <volt/volt.h>
#include <interface/volt_ctx.hpp>
#include <assert.h>
#include <string.h>

#include <fstream>
#include <algorithm>
#include <set>
#include <vector>
#include <stdlib.h>

#include <stdint.h>


#define VOLT_ARR_PTR(arr) &arr[0]
#define VOLT_ARR_COUNT(arr) sizeof(arr) / sizeof(arr[0])

#define VOLT_REQUIRE(expr) assert(expr)
#define VOLT_ASSERT(expr) assert(expr)

#define VOLT_VK_SUCCESS_ASSERT(expr) VOLT_ASSERT(expr == VK_SUCCESS)


/* ------------------------------------------------------ [ Volt Liftime ] -- */


static std::vector<char> readFile(const std::string& filename) {
  std::ifstream file(filename, std::ios::ate | std::ios::binary);

  if (!file.is_open()) {
    throw std::runtime_error("failed to open file!");
  }

  size_t fileSize = (size_t)file.tellg();
  std::vector<char> buffer(fileSize);

  file.seekg(0);
  file.read(buffer.data(), fileSize);

  file.close();

  return buffer;
}


static VKAPI_ATTR VkBool32 VKAPI_CALL
debug_callback(
  VkDebugReportFlagsEXT /* flags */,
  VkDebugReportObjectTypeEXT /* objType */,
  uint64_t /* obj */,
  size_t /* location */,
  int32_t /* code */,
  const char* /* layerPrefix */,
  const char* msg ,
  void* /* userData */)
{
  printf("Volt: %s\n", msg);

  return VK_FALSE;
}


/* -- [ Setup VK ] -- */


static void
volt_vk1_instantance_setup(
  vk1::volt_internal_instance   *vk_instance_data,
  const char                    **extensions_arr,
  uint32_t                      extensions_count,
  const char                    **validation_layers_arr,
  uint32_t                      validation_layers_count,
  void                          *window_handle)
{
  /* param check */
  VOLT_REQUIRE(vk_instance_data->instance == VK_NULL_HANDLE);
  VOLT_REQUIRE(vk_instance_data->surface == VK_NULL_HANDLE);
  VOLT_REQUIRE(vk_instance_data->debug_callback == VK_NULL_HANDLE);

  /* create instance */
  {
    const VkApplicationInfo app_info = {
      VK_STRUCTURE_TYPE_APPLICATION_INFO,   /* sType */
      nullptr,                              /* pNext */
      "Volt",                               /* pApplicationName */
      VK_MAKE_VERSION(0,0,1),               /* applicationVersion */
      "None",                               /* pEngineName */
      VK_MAKE_VERSION(0,0,1),               /* engineVersion */
      VK_API_VERSION_1_0                    /* apiVersion */
    };

    std::vector<const char*> ext_names;
    ext_names.reserve(32);

    for (uint32_t i = 0; i < extensions_count; ++i)
    {
      ext_names.emplace_back(extensions_arr[i]);
    }

    ext_names.emplace_back(VK_EXT_DEBUG_REPORT_EXTENSION_NAME);

    const VkInstanceCreateInfo create_info = {
      VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,   /* sType */
      nullptr,                                  /* pNext */
      0,                                        /* flags */
      &app_info,                                /* pApplicationInfo */
      validation_layers_count,                  /* enabledLayerCount */
      validation_layers_arr,                    /* ppEnabledLayerNames */
      (uint32_t)ext_names.size(),               /* enabledExtensionCount */
      ext_names.data(),                         /* ppEnabledExtensionNames */
    };

    VkInstance *instance = &vk_instance_data->instance;

    VOLT_VK_SUCCESS_ASSERT(vkCreateInstance(
      &create_info,
      nullptr,
      instance)
    );

    VOLT_ASSERT(instance != nullptr);
  }

  /* logging */
  {
    VkInstance instance = vk_instance_data->instance;
    VkDebugReportCallbackEXT *callback = &vk_instance_data->debug_callback;

    VOLT_REQUIRE(instance);
    VOLT_REQUIRE(callback != nullptr);

    PFN_vkCreateDebugReportCallbackEXT vkCreateDebugReportCallbackEXT = (PFN_vkCreateDebugReportCallbackEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugReportCallbackEXT");

    if (vkCreateDebugReportCallbackEXT != nullptr)
    {
      VkDebugReportFlagsEXT flags = VK_DEBUG_REPORT_ERROR_BIT_EXT | VK_DEBUG_REPORT_WARNING_BIT_EXT;

      const VkDebugReportCallbackCreateInfoEXT create_info = {
        VK_STRUCTURE_TYPE_DEBUG_REPORT_CALLBACK_CREATE_INFO_EXT,  /* sType */
        nullptr,                                                  /* pNext */
        flags,                                                    /* flags */
        debug_callback,                                           /* pfnCallback */
        nullptr,                                                  /* pUserData */
      };

      VOLT_VK_SUCCESS_ASSERT(vkCreateDebugReportCallbackEXT(
        instance,
        &create_info,
        nullptr,
        callback)
      );
    }
  }

  /* surface */
  {
    const VkInstance instance = vk_instance_data->instance;
    VkSurfaceKHR *surface = &vk_instance_data->surface;

    VOLT_REQUIRE(instance != VK_NULL_HANDLE);
    VOLT_REQUIRE(surface != nullptr);

    #ifdef _WIN32
    const PFN_vkCreateWin32SurfaceKHR vkCreateWin32SurfaceKHR = (PFN_vkCreateWin32SurfaceKHR)vkGetInstanceProcAddr(instance, "vkCreateWin32SurfaceKHR");

    if (vkCreateWin32SurfaceKHR)
    {
      const HINSTANCE hinstance = GetModuleHandle(nullptr);
      const HWND hwnd = (HWND)window_handle;

      VOLT_REQUIRE(hwnd);
      VOLT_REQUIRE(hinstance);

      const VkWin32SurfaceCreateInfoKHR create_info = {
        VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR,  /* sType */
        nullptr,                                          /* pNext */
        0,                                                /* flags */
        hinstance,                                        /* hinstance */
        hwnd,                                             /* hwnd */
      };

      const VkResult create_surface_result = vkCreateWin32SurfaceKHR(
        instance,
        &create_info,
        nullptr,
        surface
      );

      VOLT_ASSERT(create_surface_result == VK_SUCCESS);
      VOLT_ASSERT(surface != VK_NULL_HANDLE);
    }

    #else /* win32 */
    #error "Unknown platform"
    #endif
  }
}


static void
volt_vk1_device_setup(
  vk1::volt_internal_device *device_data,
  vk1::volt_internal_instance *instance_data,
  const char **device_extensions,
  unsigned device_extensions_count,
  const char **validation_layers,
  unsigned validation_layers_count
)
{
  VOLT_REQUIRE(device_data != nullptr);
  VOLT_REQUIRE(instance_data != nullptr);

  VkSurfaceKHR surface = instance_data->surface;

  VOLT_REQUIRE(surface);

  VkInstance instance = instance_data->instance;

  VOLT_REQUIRE(instance != VK_NULL_HANDLE);

  /* physical device */

  VkPresentModeKHR present_mode = VK_PRESENT_MODE_FIFO_KHR;
  VkExtent2D actual_extent;
  VkSurfaceTransformFlagBitsKHR current_transform = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;

  VkSurfaceFormatKHR surface_format = {
    VK_FORMAT_B8G8R8A8_UNORM,             /* VkFormat */
    VK_COLOR_SPACE_SRGB_NONLINEAR_KHR,    /* VkColorSpaceKHR */
  };

  {
    uint32_t device_count = 0;
    vkEnumeratePhysicalDevices(instance, &device_count, nullptr);

    std::vector<VkPhysicalDevice> devices{ device_count };
    vkEnumeratePhysicalDevices(instance, &device_count, devices.data());

    if (device_count == 0)
    {
      //return VOLT_FALSE;
    }

    /* find device */
    for (auto &device : devices)
    {
      /* check extensions */
      uint32_t ext_count;
      vkEnumerateDeviceExtensionProperties(device, nullptr, &ext_count, nullptr);

      std::vector<VkExtensionProperties> avail_ext(ext_count);
      vkEnumerateDeviceExtensionProperties(device, nullptr, &ext_count, avail_ext.data());

      bool supports_ext = true;

      for(unsigned i = 0; i < device_extensions_count; ++i)
      {
        const char *req_ext = device_extensions[i];

        bool found = false;

        for (VkExtensionProperties ext_prop : avail_ext)
        {
          if (strcmp(req_ext, ext_prop.extensionName) == 0)
          {
            found = true;
            break;
          }
        }

        if (!found)
        {
          supports_ext = false;
          break;
        }
      }

      if (!supports_ext)
      {
        continue;
      }

      /* check swap chain */
      {
        uint32_t format_count = 0;
        vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &format_count, nullptr);

        uint32_t present_mode_count = 0;
        vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &present_mode_count, nullptr);

        if(format_count == 0 || present_mode_count == 0)
        {
          continue;
        }
      }

      /* reset ids */
      for(uint32_t &id : device_data->queue_index)
      {
        id = -1;
      }

      /* get queue */
      uint32_t queue_family_count = 0;
      vkGetPhysicalDeviceQueueFamilyProperties(device, &queue_family_count, nullptr);

      std::vector<VkQueueFamilyProperties> queue_family(queue_family_count);
      vkGetPhysicalDeviceQueueFamilyProperties(device, &queue_family_count, queue_family.data());

      int device_index = 0;

      for (const auto& family : queue_family)
      {
        if (device_data->queue_index[vk1::QUEUE_GRAPHICS] == -1)
        {
          if (family.queueCount > 0 && (family.queueFlags & VK_QUEUE_GRAPHICS_BIT))
          {
            device_data->queue_index[vk1::QUEUE_GRAPHICS] = device_index;
          }
        }

        if (device_data->queue_index[vk1::QUEUE_PRESENT] == -1)
        {
          VkBool32 present_support = VK_FALSE;

          vkGetPhysicalDeviceSurfaceSupportKHR(
            device,
            device_index,
            surface,
            &present_support
          );

          if (present_support == VK_TRUE)
          {
            device_data->queue_index[vk1::QUEUE_PRESENT] = device_index;
          }
        }

        if (device_data->queue_index[vk1::QUEUE_PRESENT] != -1 && device_data->queue_index[vk1::QUEUE_GRAPHICS] != -1)
        {
          device_data->physical_device = device;
          break;
        }

        ++device_index;
      }
    }
  }

  /* present queue */
  std::vector<VkDeviceQueueCreateInfo> queue_create_infos;
  {   
    std::set<uint32_t> unique_queue_families = { device_data->queue_index[vk1::QUEUE_GRAPHICS], device_data->queue_index[vk1::QUEUE_PRESENT] };

    const size_t count = unique_queue_families.size();
    queue_create_infos.reserve(count);

    const float queue_priority = 1.f;

    for (const int queue_family : unique_queue_families)
    {
      const VkDeviceQueueCreateInfo create_info = {
        VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO, /* sType */
        nullptr,                                    /* pNext */
        0,                                          /* flags */
        queue_family,                               /* queueFamilyIndex */
        1,                                          /* queueCount */
        &queue_priority                             /* pQueuePriorities */
      };

      queue_create_infos.emplace_back(create_info);
    }

    VOLT_ASSERT(queue_create_infos.empty() == false);
  }

  /* device */
  {
    VkPhysicalDevice physical_device = device_data->physical_device;
    VkDevice *device = &device_data->device;
    VkQueue *graphics_queue = &device_data->queue[vk1::QUEUE_GRAPHICS];
    VkQueue *present_queue = &device_data->queue[vk1::QUEUE_PRESENT];

    VOLT_REQUIRE(device);
    VOLT_REQUIRE(graphics_queue);
    VOLT_REQUIRE(present_queue);
    VOLT_REQUIRE(physical_device != VK_NULL_HANDLE);
    VOLT_REQUIRE(device_data->physical_device != VK_NULL_HANDLE);
    VOLT_REQUIRE(queue_create_infos.empty() == false);

    const VkPhysicalDeviceFeatures device_features = {};

    const VkDeviceCreateInfo create_info = {
      VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,   /* sType */
      nullptr,                                /* pNext */
      0,                                      /* flags */
      (uint32_t)queue_create_infos.size(),    /* queueCreateInfoCount */
      queue_create_infos.data(),              /* pQueueCreateInfos */
      validation_layers_count,                /* enabledLayerCount */
      validation_layers,                      /* ppEnabledLayerNames */
      device_extensions_count,                /* enabledExtensionCount */
      device_extensions,                      /* ppEnabledExtensionNames */
      &device_features,                       /* pEnabledFeatures */
    };

    const VkResult success = vkCreateDevice(
      physical_device,
      &create_info,
      nullptr,
      device
    );

    VOLT_ASSERT(success == VK_SUCCESS);
    VOLT_ASSERT(device != VK_NULL_HANDLE);

    vkGetDeviceQueue(
      *device,
      device_data->queue_index[vk1::QUEUE_GRAPHICS],
      0,
      graphics_queue
    );

    VOLT_ASSERT(*graphics_queue != VK_NULL_HANDLE);

    vkGetDeviceQueue(
      *device,
      device_data->queue_index[vk1::QUEUE_PRESENT],
      0,
      present_queue
    );

    VOLT_ASSERT(*present_queue != VK_NULL_HANDLE);
  }

}


static void
volt_vk1_swap_chain_setup(
  vk1::volt_internal_device *device_data,
  vk1::volt_internal_instance *instance_data,
  vk1::volt_internal_swap_chain *swap_chain_data)
{
  /* param check */
  VOLT_REQUIRE(device_data);
  VOLT_REQUIRE(instance_data);
  VOLT_REQUIRE(swap_chain_data);

  VkSwapchainKHR *swap_chain = &swap_chain_data->swap_chain;
  VkDevice device = device_data->device;
  VkPhysicalDevice phy_device = device_data->physical_device;
  VkSurfaceKHR surface = instance_data->surface;
  VkSurfaceFormatKHR surface_format;
  VkExtent2D actual_extent;
  VkSurfaceTransformFlagBitsKHR current_transform = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;
  VkPresentModeKHR present_mode = VK_PRESENT_MODE_FIFO_KHR;

  VOLT_REQUIRE(device != VK_NULL_HANDLE);

  {
    struct SwapChainSupportDetails {
      VkSurfaceCapabilitiesKHR capabilities;
      std::vector<VkSurfaceFormatKHR> formats;
      std::vector<VkPresentModeKHR> presentModes;
    } details;

    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(phy_device, surface, &details.capabilities);

    uint32_t format_count = 0;
    vkGetPhysicalDeviceSurfaceFormatsKHR(phy_device, surface, &format_count, nullptr);

    if (format_count != 0)
    {
      details.formats.resize(format_count);
      vkGetPhysicalDeviceSurfaceFormatsKHR(phy_device, surface, &format_count, details.formats.data());
    }

    uint32_t present_mode_count = 0;
    vkGetPhysicalDeviceSurfacePresentModesKHR(phy_device, surface, &present_mode_count, nullptr);

    if (present_mode_count != 0) {
      details.presentModes.resize(present_mode_count);
      vkGetPhysicalDeviceSurfacePresentModesKHR(phy_device, surface, &present_mode_count, details.presentModes.data());
    }

    /* just pick first right now */
    surface_format = details.formats[0];

    /* extent */
    if (details.capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max()) {
      actual_extent = details.capabilities.currentExtent;
    }
    else {
      VkExtent2D actualExtent = { 800, 480 };

      actualExtent.width = std::max(details.capabilities.minImageExtent.width, std::min(details.capabilities.maxImageExtent.width, actualExtent.width));
      actualExtent.height = std::max(details.capabilities.minImageExtent.height, std::min(details.capabilities.maxImageExtent.height, actualExtent.height));

      actual_extent = actualExtent;
    }


    device_data->image_count = details.capabilities.minImageCount + 1;
    if (details.capabilities.maxImageCount > 0 && device_data->image_count > details.capabilities.maxImageCount) {
      device_data->image_count = details.capabilities.maxImageCount;
    }

    current_transform = details.capabilities.currentTransform;
  }

  /* create swap chain */
  {
    VOLT_REQUIRE(device_data->image_count > 0);

    const VkSharingMode share_mode = device_data->queue_index[vk1::QUEUE_GRAPHICS] == device_data->queue_index[vk1::QUEUE_PRESENT] ? VK_SHARING_MODE_EXCLUSIVE : VK_SHARING_MODE_CONCURRENT;
    const uint32_t index_count = share_mode == VK_SHARING_MODE_CONCURRENT ? 2 : 0;
    const uint32_t* family_indices = share_mode == VK_SHARING_MODE_CONCURRENT ? VOLT_ARR_PTR(device_data->queue_index) : nullptr;

    const VkSwapchainCreateInfoKHR create_info = {
      VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,  /* sType */
      nullptr,                                      /* pNext */
      0,                                            /* flags */
      surface,                                      /* surface */
      device_data->image_count,                     /* minImageCount */
      surface_format.format,                        /* imageFormat */
      surface_format.colorSpace,                    /* imageColorSpace */
      actual_extent,                                /* imageExtent */
      1,                                            /* imageArrayLayers */
      VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,          /* imageUsage */
      share_mode,                                   /* imageSharingMode */
      index_count,                                  /* queueFamilyIndexCount */
      family_indices,                               /* pQueueFamilyIndices */
      current_transform,                            /* preTransform */
      VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,            /* compositeAlpha */
      present_mode,                                 /* presentMode */
      VK_TRUE,                                      /* clipped */
      VK_NULL_HANDLE,                               /* oldSwapchain */
    };

    VkResult success_swap_chain = vkCreateSwapchainKHR(
      device,
      &create_info,
      nullptr,
      swap_chain
    );

    VOLT_ASSERT(success_swap_chain == VK_SUCCESS);

    uint32_t image_count = 0;

    vkGetSwapchainImagesKHR(
      device,
      *swap_chain,
      &image_count,
      nullptr
    );

    swap_chain_data->images.resize(image_count);

    vkGetSwapchainImagesKHR(
      device,
      *swap_chain,
      &image_count,
      swap_chain_data->images.data()
    );

    swap_chain_data->format = surface_format.format;
    swap_chain_data->extent = actual_extent;
  }

  /* image views */
  {
    //VOLT_REQUIRE(new_ctx->logical_device != VK_NULL_HANDLE);
    //VOLT_REQUIRE(new_ctx->swap_chain != VK_NULL_HANDLE);

    const size_t count = swap_chain_data->images.size();
    swap_chain_data->image_views.resize(count);

    for (size_t i = 0; i < count; ++i)
    {
      constexpr VkComponentMapping mapping = {
        VK_COMPONENT_SWIZZLE_IDENTITY,  /* r */
        VK_COMPONENT_SWIZZLE_IDENTITY,  /* g */
        VK_COMPONENT_SWIZZLE_IDENTITY,  /* b */
        VK_COMPONENT_SWIZZLE_IDENTITY,  /* a */
      };

      constexpr VkImageSubresourceRange sub_resource = {
        VK_IMAGE_ASPECT_COLOR_BIT,  /* aspectMask */
        0,                          /* baseMipLevel */
        1,                          /* levelCount */
        0,                          /* baseArrayLayer */
        1,                          /* layerCount */
      };

      const VkImageViewCreateInfo create_info = {
        VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,   /* sType */
        nullptr,                                    /* pNext */
        0,                                          /* flags */
        swap_chain_data->images[i],                 /* image */
        VK_IMAGE_VIEW_TYPE_2D,                      /* viewType */
        swap_chain_data->format,                    /* format */
        mapping,                                    /* components */
        sub_resource,                               /* subresourceRange */
      };

      const VkResult success = vkCreateImageView(
        device,
        &create_info,
        nullptr,
        &swap_chain_data->image_views[i]
      );

      VOLT_ASSERT(success == VK_SUCCESS);
    }

    /* semaphores */
    {
      const VkSemaphoreCreateInfo create_info = {
        VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO,  /* sType */
        nullptr,                                  /* pNext */
        0,                                        /* flags */
      };

      vkCreateSemaphore(
        device,
        &create_info,
        nullptr,
        &swap_chain_data->image_available_semaphore
      );

      vkCreateSemaphore(
        device,
        &create_info,
        nullptr,
        &swap_chain_data->render_finished_semaphore
      );
    }
  }
}


VOLT_BOOL
volt_ctx_create(
  struct volt_ctx **ctx,
  struct volt_ctx_desc *desc)
{
  /* param check */
  assert(ctx);
  assert(desc);

  volt_ctx *new_ctx = new volt_ctx;

  if (new_ctx)
  {
    memset(new_ctx, 0, sizeof(volt_ctx));

    /* vulkan */
    {
      const char *device_extensions[] = {
        VK_KHR_SWAPCHAIN_EXTENSION_NAME
      };

      #ifdef NDEBUG
      const char *validation_layers[] = {
      };
      #else
      const char *validation_layers[] = {
        "VK_LAYER_LUNARG_standard_validation",
      };
      #endif

      /* check validation layers */
      if (VOLT_ARR_COUNT(validation_layers))
      {
        printf("Searching Validation Layers\n");

        uint32_t layer_count;
        vkEnumerateInstanceLayerProperties(&layer_count, nullptr);

        std::vector<VkLayerProperties> layers(layer_count);
        vkEnumerateInstanceLayerProperties(&layer_count, layers.data());

        for (auto &vl : validation_layers)
        {
          bool found = false;

          for (auto &l : layers)
          {
            if (strcmp(vl, l.layerName) == 0)
            {
              printf("Found Layer %s\n", vl);
              found = true;
            }
          }

          if (!found)
          {
            printf("Failed to find validation layer %s\n", vl);
            assert(false);
          }
        }
      }

      /* create instance data */
      volt_vk1_instantance_setup(
        &new_ctx->instance,
        desc->extension_list,
        desc->extension_list_count,
        VOLT_ARR_PTR(validation_layers),
        VOLT_ARR_COUNT(validation_layers),
        desc->window_handle
      );

      volt_vk1_device_setup(
        &new_ctx->device,
        &new_ctx->instance,
        VOLT_ARR_PTR(device_extensions),
        VOLT_ARR_COUNT(device_extensions),
        VOLT_ARR_PTR(validation_layers),
        VOLT_ARR_COUNT(validation_layers)
      );

      volt_vk1_swap_chain_setup(
        &new_ctx->device,
        &new_ctx->instance,
        &new_ctx->swap_chain
      );

      VkInstance instance = new_ctx->instance.instance;
      VkSurfaceKHR surface = new_ctx->instance.surface;
  
      /* cmd */
      {
        VkCommandPoolCreateInfo create_info = {
          VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,         /* sType */
          nullptr,                                            /* pNext */
          0,                                                  /* flags */
          new_ctx->device.queue_index[vk1::QUEUE_GRAPHICS],   /* queueFamilyIndex */
        };

        const VkResult cmd_success = vkCreateCommandPool(
          new_ctx->device.device,
          &create_info,
          nullptr,
          &new_ctx->command_pool);

        VOLT_ASSERT(cmd_success == VK_SUCCESS);
      }

    } /* vk */

    *ctx = new_ctx;

    /* renderpass / draw */
    {
      /* shaders and pipeline */
      {
        VkShaderModule vert_shader = VK_NULL_HANDLE;
        {
          auto vertShaderCode = readFile("C:/Users/SimStim/Desktop/vert.spv");

          const VkShaderModuleCreateInfo create_info = {
            VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,  /* sType */
            nullptr,                                      /* pNext */
            0,                                            /* flags */
            (uint32_t)vertShaderCode.size(),              /* codeSize */
            (const uint32_t*)vertShaderCode.data(),       /* pCode */
          };

          const VkResult success = vkCreateShaderModule(
            new_ctx->device.device,
            &create_info,
            nullptr,
            &vert_shader);

          VOLT_ASSERT(success == VK_SUCCESS);
        }

        VkShaderModule frag_shader = VK_NULL_HANDLE;
        {
          auto fragShaderCode = readFile("C:/Users/SimStim/Desktop/frag.spv");

          const VkShaderModuleCreateInfo create_info = {
            VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,  /* sType */
            nullptr,                                      /* pNext */
            0,                                            /* flags */
            (uint32_t)fragShaderCode.size(),              /* codeSize */
            (const uint32_t*)fragShaderCode.data(),       /* pCode */
          };

          const VkResult success = vkCreateShaderModule(
            new_ctx->device.device,
            &create_info,
            nullptr,
            &frag_shader);

          VOLT_ASSERT(success == VK_SUCCESS);
        }

        const VkPipelineShaderStageCreateInfo shader_stages[] =
        {
          /* vertex shader */
          {
            VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,  /* sType */
            nullptr,                                              /* pNext */
            0,                                                    /* flags */
            VK_SHADER_STAGE_VERTEX_BIT,                           /* stage */
            vert_shader,                                          /* module */
            "main",                                               /* pName */
            nullptr,                                              /* pSpecializationInfo */
          },

          /* frag shader */
          {
            VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,  /* sType */
            nullptr,                                              /* pNext */
            0,                                                    /* flags */
            VK_SHADER_STAGE_FRAGMENT_BIT,                         /* stage */
            frag_shader,                                          /* module */
            "main",                                               /* pName */
            nullptr,                                              /* pSpecializationInfo */
          },
        };

        const VkPipelineVertexInputStateCreateInfo vertex_input_info = {
          VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO,  /* sType */
          nullptr,                                                    /* pNext */
          0,                                                          /* flags */
          0,                                                          /* vertexBindingDescriptionCount */
          nullptr,                                                    /* pVertexBindingDescriptions */
          0,                                                          /* vertexAttributeDescriptionCount */
          nullptr,                                                    /* pVertexAttributeDescriptions */
        };

        const VkPipelineInputAssemblyStateCreateInfo input_assembly = {
          VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO,  /* sType */
          nullptr,                                                      /* pNext */
          0,                                                            /* flags */
          VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST,                          /* topology */
          VK_FALSE,                                                     /* primitiveRestartEnable */
        };

        const VkViewport viewport = {
          0.f,                                        /* x */
          0.f,                                        /* y */
          (float)new_ctx->swap_chain.extent.width,    /* width */
          (float)new_ctx->swap_chain.extent.height,   /* height */
          0.f,                                        /* minDepth */
          1.f,                                        /* maxDepth */
        };

        const VkRect2D scissor = {
          { 0, 0 },                     /* offset */
          new_ctx->swap_chain.extent    /* extent */
        };

        const VkPipelineViewportStateCreateInfo viewport_state = {
          VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO,  /* sType */
          nullptr,                                                /* pNext */
          0,                                                      /* flags */
          1,                                                      /* viewportCount */
          &viewport,                                              /* pViewports */
          1,                                                      /* scissorCount */
          &scissor,                                               /* pScissors */
        };

        const VkPipelineRasterizationStateCreateInfo rasterizer = {
          VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO,   /* sType */
          nullptr,                                                      /* pNext */
          0,                                                            /* flags */
          VK_FALSE,                                                     /* depthClampEnable */
          VK_FALSE,                                                     /* rasterizerDiscardEnable */
          VK_POLYGON_MODE_FILL,                                         /* polygonMode */
          VK_CULL_MODE_BACK_BIT,                                        /* cullMode */
          VK_FRONT_FACE_CLOCKWISE,                                      /* frontFace */
          VK_FALSE,                                                     /* depthBiasEnable */
          0.f,                                                          /* depthBiasConstantFactor */
          0.f,                                                          /* depthBiasClamp */
          0.f,                                                          /* depthBiasSlopeFactor */
          1.f,                                                          /* lineWidth */
        };

        const VkPipelineMultisampleStateCreateInfo multisampling = {
          VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO,   /* sType */
          nullptr,                                                    /* pNext */
          0,                                                          /* flags */
          VK_SAMPLE_COUNT_1_BIT,                                      /* rasterizationSamples */
          VK_FALSE,                                                   /* sampleShadingEnable */
          1.f,                                                        /* minSampleShading */
          nullptr,                                                    /* pSampleMask */
          VK_FALSE,                                                   /* alphaToCoverageEnable */
          VK_FALSE,                                                   /* alphaToOneEnable */
        };

        constexpr uint32_t vk_rgba_mask =
          VK_COLOR_COMPONENT_R_BIT |
          VK_COLOR_COMPONENT_G_BIT |
          VK_COLOR_COMPONENT_B_BIT |
          VK_COLOR_COMPONENT_A_BIT;

        const VkPipelineColorBlendAttachmentState color_blend_attachment = {
          VK_FALSE,                 /* blendEnable */
          VK_BLEND_FACTOR_ONE,      /* srcColorBlendFactor */
          VK_BLEND_FACTOR_ZERO,     /* dstColorBlendFactor */
          VK_BLEND_OP_ADD,          /* colorBlendOp */
          VK_BLEND_FACTOR_ONE,      /* srcAlphaBlendFactor */
          VK_BLEND_FACTOR_ZERO,     /* dstAlphaBlendFactor */
          VK_BLEND_OP_ADD,          /* alphaBlendOp */
          vk_rgba_mask,             /* colorWriteMask */
        };

        const VkPipelineColorBlendStateCreateInfo color_blending = {
          VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO,   /* sType */
          nullptr,                                                    /* pNext */
          0,                                                          /* flags */
          VK_FALSE,                                                   /* logicOpEnable */
          VK_LOGIC_OP_COPY,                                           /* logicOp */
          1,                                                          /* attachmentCount */
          &color_blend_attachment,                                    /* pAttachments */
          { 0.f, 0.f, 0.f, 0.f },                                     /* blendConstants[4] */
        };

        const VkPipelineLayoutCreateInfo pipeline_layout_info = {
          VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,  /* sType */
          nullptr,                                        /* pNext */
          0,                                              /* flags */
          0,                                              /* setLayoutCount */
          nullptr,                                        /* pSetLayouts */
          0,                                              /* pushConstantRangeCount */
          nullptr,                                        /* pPushConstantRanges */
        };

        VkResult success = vkCreatePipelineLayout(
          new_ctx->device.device,
          &pipeline_layout_info,
          nullptr,
          &new_ctx->pipeline_layout
        );

        VOLT_ASSERT(success == VK_SUCCESS);

        //}

        /* renderpass */
        //{
        const VkAttachmentDescription color_attachment = {
          0,                                /* flags */
          new_ctx->swap_chain.format,       /* format */
          VK_SAMPLE_COUNT_1_BIT,            /* samples */
          VK_ATTACHMENT_LOAD_OP_CLEAR,      /* loadOp */
          VK_ATTACHMENT_STORE_OP_STORE,     /* storeOp */
          VK_ATTACHMENT_LOAD_OP_DONT_CARE,  /* stencilLoadOp */
          VK_ATTACHMENT_STORE_OP_DONT_CARE, /* stencilStoreOp */
          VK_IMAGE_LAYOUT_UNDEFINED,        /* initialLayout */
          VK_IMAGE_LAYOUT_PRESENT_SRC_KHR,  /* finalLayout */
        };

        const VkAttachmentReference color_attachment_ref = {
          0,                                        /* attachment */
          VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL, /* layout */
        };

        const VkSubpassDescription subpass = {
          0,                                /* flags */
          VK_PIPELINE_BIND_POINT_GRAPHICS,  /* pipelineBindPoint */
          0,                                /* inputAttachmentCount */
          nullptr,                          /* pInputAttachments */
          1,                                /* colorAttachmentCount */
          &color_attachment_ref,            /* pColorAttachments */
          nullptr,                          /* pResolveAttachments */
          nullptr,                          /* pDepthStencilAttachment */
          0,                                /* preserveAttachmentCount */
          nullptr,                          /* pPreserveAttachments */
        };

        const VkRenderPassCreateInfo render_pass_info = {
          VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO,    /* sType */
          nullptr,                                      /* pNext */
          0,                                            /* flags */
          1,                                            /* attachmentCount */
          &color_attachment,                            /* pAttachments */
          1,                                            /* subpassCount */
          &subpass,                                     /* pSubpasses */
          0,                                            /* dependencyCount */
          nullptr,                                      /* pDependencies */
        };

        const VkResult success2 = vkCreateRenderPass(
          new_ctx->device.device,
          &render_pass_info,
          nullptr,
          &new_ctx->render_pass
        );

        VOLT_ASSERT(success2 == VK_SUCCESS);

        const VkGraphicsPipelineCreateInfo pipeline_info = {
          VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO,      /* sType */
          nullptr,                                              /* pNext */
          0,                                                    /* flags */
          VOLT_ARR_COUNT(shader_stages),                        /* stageCount */
          VOLT_ARR_PTR(shader_stages),                          /* pStages */
          &vertex_input_info,                                   /* pVertexInputState */
          &input_assembly,                                      /* pInputAssemblyState */
          nullptr,                                              /* pTessellationState */
          &viewport_state,                                      /* pViewportState */
          &rasterizer,                                          /* pRasterizationState */
          &multisampling,                                       /* pMultisampleState */
          nullptr,                                              /* pDepthStencilState */
          &color_blending,                                      /* pColorBlendState */
          nullptr,                                              /* pDynamicState */
          new_ctx->pipeline_layout,                             /* layout */
          new_ctx->render_pass,                                 /* renderPass */
          0,                                                    /* subpass */
          VK_NULL_HANDLE,                                       /* basePipelineHandle */
          -1,                                                   /* basePipelineIndex */
        };

        const VkResult check = vkCreateGraphicsPipelines(
          new_ctx->device.device,
          VK_NULL_HANDLE,
          1,
          &pipeline_info,
          nullptr,
          &new_ctx->graphics_pipeline
        );

        VOLT_ASSERT(check == VK_SUCCESS);

        vkDestroyShaderModule(new_ctx->device.device, frag_shader, nullptr);
        vkDestroyShaderModule(new_ctx->device.device, vert_shader, nullptr);
      }

      {
        new_ctx->swap_chain.framebuffers.resize(new_ctx->swap_chain.image_views.size());

        for (size_t i = 0; i < new_ctx->swap_chain.framebuffers.size(); ++i)
        {
          const VkFramebufferCreateInfo create_info = {
            VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO,  /* sType */
            nullptr,                                    /* pNext */
            0,                                          /* flags */
            new_ctx->render_pass,                       /* renderPass */
            1,                                          /* attachmentCount */
            &new_ctx->swap_chain.image_views[i],        /* pAttachments */
            new_ctx->swap_chain.extent.width,           /* width */
            new_ctx->swap_chain.extent.height,          /* height */
            1,                                          /* layers */
          };

          const VkResult fbo_success = vkCreateFramebuffer(
            new_ctx->device.device,
            &create_info,
            nullptr,
            &new_ctx->swap_chain.framebuffers[i]
          );

          VOLT_ASSERT(fbo_success == VK_SUCCESS);
        }
      }

      /* cmd buffer */
      {
        new_ctx->command_buffers.resize(new_ctx->swap_chain.framebuffers.size());

        const VkCommandBufferAllocateInfo alloc_info = {
          VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO, /* sType */
          nullptr,                                        /* pNext */
          new_ctx->command_pool,                          /* commandPool */
          VK_COMMAND_BUFFER_LEVEL_PRIMARY,                /* level */
          (uint32_t)new_ctx->command_buffers.size(),      /* commandBufferCount */
        };

        const VkResult result = vkAllocateCommandBuffers(
          new_ctx->device.device,
          &alloc_info,
          new_ctx->command_buffers.data()
        );

        VOLT_ASSERT(result == VK_SUCCESS);
      }

      /* cmds */
      {
        for (int i = 0; i < new_ctx->command_buffers.size(); ++i)
        {
          const VkCommandBufferBeginInfo create_info = {
            VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,  /* sType */
            nullptr,                                      /* pNext */
            VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT, /* flags */
            nullptr,                                      /* pInheritanceInfo */
          };

          vkBeginCommandBuffer(new_ctx->command_buffers[i], &create_info);

          const VkClearValue clear_color = { 0.f, 0.f, 0.f, 1.f };

          const VkRenderPassBeginInfo render_pass_info = {
            VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO,   /* sType */
            nullptr,                                    /* pNext */
            new_ctx->render_pass,                       /* renderPass */
            new_ctx->swap_chain.framebuffers[i],        /* framebuffer */
            { { 0,0 }, new_ctx->swap_chain.extent },    /* renderArea */
            1,                                          /* clearValueCount */
            &clear_color,                               /* pClearValues */
          };

          vkCmdBeginRenderPass(
            new_ctx->command_buffers[i],
            &render_pass_info,
            VK_SUBPASS_CONTENTS_INLINE
          );

          vkCmdBindPipeline(new_ctx->command_buffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, new_ctx->graphics_pipeline);

          vkCmdDraw(new_ctx->command_buffers[i], 3, 1, 0, 0);
          vkCmdEndRenderPass(new_ctx->command_buffers[i]);

          const VkResult cmd_success = vkEndCommandBuffer(new_ctx->command_buffers[i]);
          VOLT_ASSERT(cmd_success == VK_SUCCESS);
        }
      }


    }
    return VOLT_TRUE;
  }
  return VOLT_FALSE;
}



VOLT_BOOL
volt_ctx_destroy(
  struct volt_ctx **ctx)
{
  VOLT_REQUIRE(ctx);
  VOLT_REQUIRE(*ctx);

  VkDevice device = (*ctx)->device.device;
  VkInstance instance = (*ctx)->instance.instance;

  vkDestroyCommandPool((*ctx)->device.device, (*ctx)->command_pool, nullptr);
  vkDestroyRenderPass((*ctx)->device.device, (*ctx)->render_pass, nullptr);
  vkDestroyPipelineLayout((*ctx)->device.device, (*ctx)->pipeline_layout, nullptr);
  
  /* clean up swap chain */
  {
    vk1::volt_internal_swap_chain *swap_data = &(*ctx)->swap_chain;

    VOLT_REQUIRE(swap_data);

    for (auto &img : (*ctx)->swap_chain.image_views)
    {
      vkDestroyImageView(device, img, nullptr);
    }

    for (auto &fbo : (*ctx)->swap_chain.framebuffers)
    {
      vkDestroyFramebuffer(device, fbo, nullptr);
    }

    vkDestroySemaphore(device, swap_data->image_available_semaphore, nullptr);
    vkDestroySemaphore(device, swap_data->render_finished_semaphore, nullptr);
    vkDestroySwapchainKHR(device, swap_data->swap_chain, nullptr);
  }
  
  /* clean up device */
  {
    vk1::volt_internal_device *dev_data = &(*ctx)->device;
  
    VOLT_REQUIRE(dev_data);

    vkDestroyDevice(device, nullptr);
  }

  /* clean up instance */
  {
    vk1::volt_internal_instance *inst_data = &(*ctx)->instance;

    VOLT_REQUIRE(inst_data);

    PFN_vkDestroyDebugReportCallbackEXT vkDestroyDebugReportCallbackEXT = (PFN_vkDestroyDebugReportCallbackEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugReportCallbackEXT");

    if (vkDestroyDebugReportCallbackEXT != nullptr)
    {
      vkDestroyDebugReportCallbackEXT(instance, inst_data->debug_callback, nullptr);
    }

    vkDestroySurfaceKHR(instance, inst_data->surface, nullptr);
    vkDestroyInstance(instance, nullptr);
  }

  return VOLT_FALSE;
}


void
volt_ctx_execute(
  struct volt_ctx *ctx)
{
  assert(ctx);

  uint32_t image_index;

  vkAcquireNextImageKHR(
    ctx->device.device,
    ctx->swap_chain.swap_chain,
    UINT64_MAX,
    ctx->swap_chain.image_available_semaphore,
    VK_NULL_HANDLE,
    &image_index
  );

  VkSemaphore wait_sem[] = { ctx->swap_chain.image_available_semaphore };
  VkPipelineStageFlags wait_stage[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
  VkSemaphore signal_sem[] = { ctx->swap_chain.render_finished_semaphore };

  const VkSubmitInfo submit_info = {
    VK_STRUCTURE_TYPE_SUBMIT_INFO,      /* sType */
    nullptr,                            /* pNext */
    VOLT_ARR_COUNT(wait_sem),           /* waitSemaphoreCount */
    VOLT_ARR_PTR(wait_sem),             /* pWaitSemaphores */
    VOLT_ARR_PTR(wait_stage),           /* pWaitDstStageMask */
    1,                                  /* commandBufferCount */
    &ctx->command_buffers[image_index], /* pCommandBuffers */
    VOLT_ARR_COUNT(signal_sem),         /* signalSemaphoreCount */
    VOLT_ARR_PTR(signal_sem),           /* pSignalSemaphores */
  };

  auto res = vkQueueSubmit(ctx->device.queue[vk1::QUEUE_GRAPHICS], 1, &submit_info, VK_NULL_HANDLE);
  VOLT_ASSERT(res == VK_SUCCESS);

  VkPresentInfoKHR presentInfo = {};
  presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

  presentInfo.waitSemaphoreCount = 1;
  presentInfo.pWaitSemaphores = signal_sem;

  VkSwapchainKHR swapChains[] = { ctx->swap_chain.swap_chain };
  presentInfo.swapchainCount = 1;
  presentInfo.pSwapchains = swapChains;

  presentInfo.pImageIndices = &image_index;

  vkQueuePresentKHR(ctx->device.queue[vk1::QUEUE_PRESENT], &presentInfo);

  vkQueueWaitIdle(ctx->device.queue[vk1::QUEUE_PRESENT]);
}