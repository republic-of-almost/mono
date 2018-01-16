#include <vulkan/vulkan.h>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <volt/volt.h>

#include <string.h>


int
main()
{
  glfwInit();

  glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
  glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
  GLFWwindow* window = glfwCreateWindow(800, 480, "My Title", NULL, NULL);

  struct volt_ctx *volt_ctx = NULL;
  struct volt_ctx_desc volt_desc;
  memset(&volt_desc, 0, sizeof(volt_desc));
  volt_desc.extension_list = glfwGetRequiredInstanceExtensions(&volt_desc.extension_list_count);
  volt_desc.window_handle = glfwGetWin32Window(window);

  volt_ctx_create(&volt_ctx, &volt_desc);

  volt_vert_attr vertex[2];
  vertex[0] = VOLT_VERTEX_FLOAT2;
  vertex[1] = VOLT_VERTEX_FLOAT3;

  struct volt_vertex_desc vert_desc;
  vert_desc.attr = vertex;
  vert_desc.count = 2;

  unsigned vertex_input = volt_vertex_create(volt_ctx, &vert_desc);

  float vertex_data[15];
  vertex_data[0] = +0.0f;
  vertex_data[1] = -0.5f;
  vertex_data[2] = +1.0f;
  vertex_data[3] = +1.0f;
  vertex_data[4] = +1.0f;

  vertex_data[5] = +0.5f;
  vertex_data[6] = +0.5f;
  vertex_data[7] = +0.0f;
  vertex_data[8] = +1.0f;
  vertex_data[9] = +0.0f;

  vertex_data[10] = -0.5f;
  vertex_data[11] = +0.5f;
  vertex_data[12] = +0.0f;
  vertex_data[13] = +0.0f;
  vertex_data[14] = +1.0f;

  struct volt_vertex_buffer_desc vbo_desc;
  vbo_desc.data = vertex_data;
  vbo_desc.data_count = 15;

  unsigned vertex_buffer = volt_vertex_buffer_create(volt_ctx, &vbo_desc);

  while (!glfwWindowShouldClose(window))
  {
    glfwPollEvents();

    volt_ctx_execute(volt_ctx);
  }

  volt_ctx_destroy(&volt_ctx);

  glfwDestroyWindow(window);
  glfwTerminate();

  return 0;
}