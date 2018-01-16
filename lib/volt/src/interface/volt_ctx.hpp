#ifndef VOLT_CTX_INCLUDED_A42358E2_3D39_49A9_954A_DDBE1AA3F253
#define VOLT_CTX_INCLUDED_A42358E2_3D39_49A9_954A_DDBE1AA3F253


#ifdef _WIN32
#define VK_USE_PLATFORM_WIN32_KHR
#endif


#include <vulkan/vulkan.h>
#include <vector>


namespace vk1 {


struct volt_internal_subpass
{

};


struct volt_internal_renderpass
{
  /* array */ volt_internal_subpass subpass;
};


enum {
  QUEUE_GRAPHICS = 0,
  QUEUE_PRESENT,

  QUEUE_COUNT,
};


struct volt_internal_device
{
  VkPhysicalDevice physical_device;
  VkDevice device;

  VkQueue queue[QUEUE_COUNT];
  uint32_t queue_index[QUEUE_COUNT];

  int image_count;
};


struct volt_internal_swap_chain
{
  VkSwapchainKHR            swap_chain;
  VkFormat                  format;
  VkExtent2D                extent;

  VkSemaphore               image_available_semaphore;
  VkSemaphore               render_finished_semaphore;

  std::vector<VkImage>       images;
  std::vector<VkImageView>   image_views;
  std::vector<VkFramebuffer> framebuffers;
};


struct volt_internal_instance
{
  VkInstance                instance;
  VkSurfaceKHR              surface;
  VkDebugReportCallbackEXT  debug_callback;
};


struct volt_internal_vertex_desc
{
  volt_vert_attr attr[5];
  unsigned count;
};


} // ns


struct volt_ctx
{
  vk1::volt_internal_instance     instance;
  vk1::volt_internal_device       device;
  vk1::volt_internal_swap_chain   swap_chain;

  std::vector<vk1::volt_internal_renderpass> renderpasses;
  
  /* render stuff */
  VkPipelineLayout pipeline_layout;
  VkPipeline graphics_pipeline;
  VkCommandPool command_pool;
  
  VkRenderPass render_pass;
  std::vector<VkCommandBuffer> command_buffers;

  std::vector<volt_internal_vertex_desc> input;

  //VkSemaphore image_available_semaphore;
  //VkSemaphore render_finished_semaphore;
};


#endif /* inc guard */