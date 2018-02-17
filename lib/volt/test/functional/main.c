
#include <GLFW/glfw3.h>

#include <volt/volt.h>
#include <roa_lib/fundamental.h>
#include <string.h>


/* app stuff */
GLFWwindow* window;
void app_create();
void app_destroy();
ROA_BOOL app_tick();


int
main()
{
  app_create();

  volt_ctx_t ctx = VOLT_NULL;
  volt_ctx_create(&ctx);

  volt_vbo_t vbo = VOLT_NULL;
  volt_ibo_t ibo = VOLT_NULL;
  volt_texture_t texture_1 = VOLT_NULL;
  volt_texture_t texture_2 = VOLT_NULL;
  volt_program_t program = VOLT_NULL;
  volt_input_t vert_input = VOLT_NULL;
  volt_rasterizer_t rasterizer = VOLT_NULL;

  /* create some assets */
  {
    /* textures */
    struct volt_texture_desc tex_desc_1;
    tex_desc_1.dimentions = VOLT_TEXTURE_2D;
    tex_desc_1.format = VOLT_COLOR_RGB;
    tex_desc_1.mip_maps = VOLT_FALSE;
    tex_desc_1.sampling = VOLT_SAMPLING_BILINEAR;

    struct volt_texture_desc tex_desc_2;
    tex_desc_2.dimentions = VOLT_TEXTURE_2D;
    tex_desc_2.format = VOLT_COLOR_RGB;
    tex_desc_2.mip_maps = VOLT_FALSE;
    tex_desc_2.sampling = VOLT_SAMPLING_BILINEAR;

    volt_texture_create(ctx, &texture_1, &tex_desc_1);
    volt_texture_create(ctx, &texture_2, &tex_desc_2);

    /* vbo */
    float verts[] = {
      -0.5f,  0.5f, 1.0f, 0.0f, 0.0f, // Top-left
      0.5f,  0.5f, 0.0f, 1.0f, 0.0f, // Top-right
      0.5f, -0.5f, 0.0f, 0.0f, 1.0f, // Bottom-right
      -0.5f, -0.5f, 1.0f, 1.0f, 1.0f  // Bottom-left
    };

    struct volt_vbo_desc vbo_desc;
    vbo_desc.data = ROA_ARR_DATA(verts);
    vbo_desc.count = ROA_ARR_COUNT(verts);

    volt_vertex_buffer_create(ctx, &vbo, &vbo_desc);

    /* ibo */
    unsigned index[] = {
      0,1,2,
      2,3,0
    };

    struct volt_ibo_desc ibo_desc;
    ibo_desc.data = ROA_ARR_DATA(index);
    ibo_desc.count = ROA_ARR_COUNT(index);

    volt_index_buffer_create(ctx, &ibo, &ibo_desc);

    const char *vert_src = ""
      "/* Vert */\n"
      "#version 400 core\n"

      "layout(location=0) in vec2 position;\n"
      "layout(location=1) in vec3 color;\n"

      "out vec3 Color;\n"
      "void main() {\n"
        "Color = color;\n"
        "gl_Position = vec4(position, 0.0, 1.0);\n"
      "}\n";

    const char* frag_src = ""
      "/* Frag */\n"
      "#version 400 core\n"
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

  while (app_tick())
  {
    static int count = 0;
    ++count;

    /* draw some stuff */
    {
      volt_renderpass_t rp;
      volt_renderpass_create(ctx, &rp);

	    volt_renderpass_bind_rasterizer(rp, rasterizer);
      volt_renderpass_bind_input_format(rp, vert_input);
      volt_renderpass_bind_vertex_buffer(rp, vbo);
      volt_renderpass_bind_index_buffer(rp, ibo);
      volt_renderpass_bind_program(rp, program);

      volt_renderpass_draw(rp);

      volt_renderpass_commit(ctx, &rp);
    }

    volt_ctx_execute(ctx);
  }

  volt_ctx_destroy(&ctx);

  app_destroy();

  return 0;
}


/* app details */


void
app_create()
{
	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);

	window = glfwCreateWindow(800, 480, "My Title", NULL, NULL);
	glfwMakeContextCurrent(window);

  printf("OpenGL version is (%s)\n", glGetString(GL_VERSION));
}


ROA_BOOL
app_tick()
{
	glfwPollEvents();
	glfwSwapBuffers(window);

	return glfwWindowShouldClose(window) ? ROA_FALSE : ROA_TRUE;
}


void
app_destroy()
{
	glfwDestroyWindow(window);
	glfwTerminate();

	window = ROA_NULL;
}