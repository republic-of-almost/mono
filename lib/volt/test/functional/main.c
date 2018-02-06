
#include <GLFW/glfw3.h>
#include <volt/volt.h>

#include <string.h>


int
main()
{
  glfwInit();

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);

  GLFWwindow* window = glfwCreateWindow(800, 480, "My Title", NULL, NULL);
  glfwMakeContextCurrent(window);

  volt_ctx_t ctx = VOLT_NULL;
  volt_ctx_create(&ctx);

  /* create some assets */
  {
    float verts[6];
    verts[0] = 0.f;
    verts[1] = 0.5f;
    verts[2] = 0.5f;
    verts[3] = -0.5f;
    verts[4] = -0.5f;
    verts[5] = -0.5f;

    volt_vbo_t vbo = VOLT_NULL;
    struct volt_vbo_desc desc;
    desc.data = verts;
    desc.count = 6;

    volt_vertex_buffer_create(ctx, &vbo, &desc);

    volt_ctx_execute(ctx);
  }

  while (!glfwWindowShouldClose(window))
  {
    /* draw some stuff */
    {
      volt_renderpass_t rp;
      volt_renderpass_create(&rp);

      

      volt_renderpass_commit(&rp);
    }

    volt_ctx_execute(ctx);

    glfwPollEvents();
    glfwSwapBuffers(window);
  }

  volt_ctx_destroy(&ctx);

  glfwDestroyWindow(window);
  glfwTerminate();

  return 0;
}