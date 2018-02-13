
#include <GLFW/glfw3.h>
#include <volt/volt.h>
#include <roa_lib/fundamental.h>
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

  volt_vbo_t vbo = VOLT_NULL;
  volt_program_t program = VOLT_NULL;
  volt_input_t vert_input = VOLT_NULL;

  /* create some assets */
  {
    /*  */
    float verts[] = {
      0.f, 0.5f, 1.0f, 0.0f, 0.0f,
      0.5f, -0.5f, 0.f, 1.f, 0.f,
      -0.5f, -0.5f, 0.f, 0.f, 1.f,
    };

    struct volt_vbo_desc desc;
    desc.data = ROA_ARR_DATA(verts);
    desc.count = ROA_ARR_COUNT(verts);

    volt_vertex_buffer_create(ctx, &vbo, &desc);

    const char *vert_src = ""
      "#version 450 core\n"

      "layout(location=0) in vec2 position;\n"
      "layout(location=1) in vec3 color;\n"

      "out vec3 Color;\n"
      "void main() {\n"
        "Color = color;\n"
        "gl_Position = vec4(position, 0.0, 1.0);\n"
      "}\n";

    const char* frag_src = ""
      "#version 450 core\n"
      "in vec3 Color;\n"
      "out vec4 outColor;\n"
      "void main()\n"
      "{\n"
        "outColor = vec4(Color, 1.0);\n"
      "}\n";

    const char **stages[2] = {vert_src, frag_src};
    const volt_shader_stage *stage_types[2] = { VOLT_SHD_VERTEX, VOLT_SHD_FRAGMENT };

    struct volt_program_desc shd_desc;
    shd_desc.shader_stages_src = stages;
    shd_desc.shader_stages_type = stage_types;
    shd_desc.stage_count = 2;

    volt_program_create(ctx, &program, &shd_desc);

    volt_input_attribute input_fmt[] = {
      VOLT_INPUT_FLOAT2,
      VOLT_INPUT_FLOAT3,
    };

    struct volt_input_desc input_desc;
    input_desc.attributes = ROA_ARR_DATA(input_fmt);
    input_desc.count = ROA_ARR_COUNT(input_fmt);

    volt_input_create(ctx, &vert_input, &input_desc);

    volt_ctx_execute(ctx);
  }

  while (!glfwWindowShouldClose(window))
  {
    static int count = 0;
    ++count;

    /* draw some stuff */
    {
      volt_renderpass_t rp;
      volt_renderpass_create(ctx, &rp);

      volt_renderpass_bind_input_format(rp, vert_input);
      volt_renderpass_bind_vertex_buffer(rp, vbo);
      volt_renderpass_bind_program(rp, program);

      volt_renderpass_draw(rp);

      volt_renderpass_commit(ctx, &rp);
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