#include <roa_renderer/roa_renderer.h>
#include <ctx/ctx.h>
#include <roa_lib/alloc.h>
#include <roa_lib/assert.h>
#include <roa_lib/array.h>
#include <roa_lib/log.h>
#include <volt/utils/prim_model.h>


/* ---------------------------------------------------------- [ Lifetime ] -- */


ROA_BOOL
roa_renderer_ctx_create(
	roa_renderer_ctx_t *ctx,
  struct roa_renderer_desc *desc)
{
	/* param check */
	ROA_ASSERT(ctx);

	struct roa_renderer_ctx *new_ctx = roa_zalloc(sizeof(*new_ctx));
	ROA_ASSERT(new_ctx);

  unsigned count = 1 << 10;

	printf("0");

  roa_array_create_with_capacity(new_ctx->mesh_camera_data, count);
  roa_array_create_with_capacity(new_ctx->mesh_rendering_data, count);
	roa_array_create_with_capacity(new_ctx->camera_id, count);
	roa_array_create_with_capacity(new_ctx->camera, count);
	roa_array_create_with_capacity(new_ctx->renderable_id, count);
	roa_array_create_with_capacity(new_ctx->renderable, count);
  roa_array_create_with_capacity(new_ctx->tasks, count);
  
	volt_ctx_create(&new_ctx->volt_ctx);

	*ctx = new_ctx;

	printf("1");

	/* temp create some dummy data */
  /* create uniforms */
  {
    struct volt_uniform_desc uni_desc_1;
    uni_desc_1.data_type = VOLT_DATA_MAT4;
    uni_desc_1.count = 1;

    volt_uniform_create(new_ctx->volt_ctx, &new_ctx->proj_data, &uni_desc_1);
    ROA_ASSERT(new_ctx->proj_data);

    struct volt_uniform_desc uni_desc_2;
    uni_desc_2.data_type = VOLT_DATA_MAT4;
    uni_desc_2.count = 1;

    volt_uniform_create(new_ctx->volt_ctx, &new_ctx->view_data, &uni_desc_2);
    ROA_ASSERT(new_ctx->view_data);

    struct volt_uniform_desc uni_desc_3;
    uni_desc_3.data_type = VOLT_DATA_MAT4;
    uni_desc_3.count = 1;

    volt_uniform_create(new_ctx->volt_ctx, &new_ctx->world_data, &uni_desc_2);
    ROA_ASSERT(new_ctx->world_data);
  }

	printf("2");

  /* create some assets */
  {
    int x, y, n;

    /* textures */
    struct volt_texture_desc tex_desc_1;
    tex_desc_1.dimentions = VOLT_TEXTURE_2D;
    tex_desc_1.mip_maps = VOLT_FALSE;
    tex_desc_1.sampling = VOLT_SAMPLING_BILINEAR;

    char file_path_01[2048];
    memset(file_path_01, 0, sizeof(file_path_01));
    strcat(file_path_01, roa_exe_dir());
    strcat(file_path_01, "assets/volt_func/dev_tex_01.png");

    stbi_set_flip_vertically_on_load(1);

    tex_desc_1.data = (void*)stbi_load(
      file_path_01,
      &x,
      &y,
      &n,
      0
    );

    tex_desc_1.width = x;
    tex_desc_1.height = y;
    tex_desc_1.format = n > 3 ? VOLT_COLOR_RGBA : VOLT_COLOR_RGB;

    struct volt_texture_desc tex_desc_2;
    tex_desc_2.dimentions = VOLT_TEXTURE_2D;
    tex_desc_2.mip_maps = VOLT_FALSE;
    tex_desc_2.sampling = VOLT_SAMPLING_BILINEAR;

    char file_path_02[2048];
    memset(file_path_02, 0, sizeof(file_path_02));
    strcat(file_path_02, roa_exe_dir());
    strcat(file_path_02, "assets/volt_func/dev_tex_02.png");

    tex_desc_2.data = (void*)stbi_load(
      file_path_02,
      &x,
      &y,
      &n,
      0
    );

    tex_desc_2.width = x;
    tex_desc_2.height = y;
    tex_desc_2.format = n > 3 ? VOLT_COLOR_RGBA : VOLT_COLOR_RGB;

    volt_texture_create(new_ctx->volt_ctx, &new_ctx->texture_1, &tex_desc_1);
    volt_texture_create(new_ctx->volt_ctx, &new_ctx->texture_2, &tex_desc_2);

    /* vbo */
    volt_vert_desc vert_desc[] = {
      VOLT_VERT_POSITION, VOLT_NORMAL, VOLT_UV,
    };

    float verts[1024];
    unsigned vert_count = 0;

    volt_util_generate_cube(
      ROA_ARR_DATA(vert_desc),
      ROA_ARR_COUNT(vert_desc),
      0.5f,
      0.5f,
      0.5f,
      ROA_ARR_DATA(verts),
      &vert_count
    );

    struct volt_vbo_desc vbo_desc;
    vbo_desc.data = ROA_ARR_DATA(verts);
    vbo_desc.count = vert_count;

    volt_vertex_buffer_create(new_ctx->volt_ctx, &new_ctx->vbo, &vbo_desc);

    /* ibo */
    const unsigned index[] = {
      0,1,2,
      2,3,0
    };

    struct volt_ibo_desc ibo_desc;
    ibo_desc.data = ROA_ARR_DATA(index);
    ibo_desc.count = ROA_ARR_COUNT(index);

    volt_index_buffer_create(new_ctx->volt_ctx, &new_ctx->ibo, &ibo_desc);

    const char *vert_src = ""

      "/* Vert */\n"
      "#version 400 core\n"

      "layout(location=0) in vec3 position;\n"
      "layout(location=1) in vec3 color;\n"
      "layout(location=2) in vec2 texcoord;\n"

      "uniform mat4 model;\n"
      "uniform mat4 view;\n"
      "uniform mat4 proj;\n"

      "out vec3 oColor;\n"
      "out vec2 oTexcoord;\n"

      "void main() {\n"
      "oColor = color;\n"
      "oTexcoord = texcoord;\n"
      "gl_Position = proj * view * model * vec4(position, 1.0);\n"
      "}\n";

    const char* frag_src = ""
      "/* Frag */\n"
      "#version 400 core\n"
      "in vec3 oColor;\n"
      "in vec2 oTexcoord;\n"

      "uniform sampler2D texKitten;\n"
      "uniform sampler2D texPuppy;\n"

      "out vec4 outColor;\n"

      "void main()\n"
      "{\n"
      "outColor = mix(texture(texKitten, oTexcoord), texture(texPuppy, oTexcoord), length(oColor));\n"
      "outColor = vec4(abs(oColor), 1);"
      "}\n";

    const char *stages[2];
    stages[0] = vert_src;
    stages[1] = frag_src;

    volt_shader_stage stage_types[2];
    stage_types[0] = VOLT_SHD_VERTEX;
    stage_types[1] = VOLT_SHD_FRAGMENT;

    struct volt_program_desc shd_desc;
    shd_desc.shader_stages_src = stages;
    shd_desc.shader_stages_type = stage_types;
    shd_desc.stage_count = 2;

    volt_program_create(new_ctx->volt_ctx, &new_ctx->program, &shd_desc);

    volt_input_attribute input_fmt[] = {
      VOLT_INPUT_FLOAT3, /* pos */
      VOLT_INPUT_FLOAT3, /* color */
      VOLT_INPUT_FLOAT2, /* tex c */
    };

    struct volt_input_desc input_desc;
    input_desc.attributes = ROA_ARR_DATA(input_fmt);
    input_desc.count = ROA_ARR_COUNT(input_fmt);

    volt_input_create(new_ctx->volt_ctx, &new_ctx->input_format, &input_desc);

    volt_ctx_execute(new_ctx->volt_ctx);
  }


	return new_ctx ? ROA_TRUE : ROA_FALSE;
}


void
roa_renderer_ctx_execute(
	roa_renderer_ctx_t ctx)
{
	/* param check */
	ROA_ASSERT(ctx);

	ctx->render = ROA_FALSE;
	
	volt_ctx_execute(ctx->volt_ctx);
}


void
roa_renderer_ctx_destroy(
	roa_renderer_ctx_t *ctx)
{
	/* param check */
	ROA_ASSERT(ctx);
	ROA_ASSERT(*ctx);
	
	struct roa_renderer_ctx *kill_ctx = *ctx;

  roa_array_destroy(kill_ctx->mesh_camera_data);
  roa_array_destroy(kill_ctx->mesh_rendering_data);
  roa_array_destroy(kill_ctx->tasks);
	roa_array_destroy(kill_ctx->renderable);
	roa_array_destroy(kill_ctx->renderable_id);
	roa_array_destroy(kill_ctx->camera);
	roa_array_destroy(kill_ctx->camera_id);

  volt_ctx_destroy(&kill_ctx->volt_ctx);

	roa_free(kill_ctx);

	*ctx = ROA_NULL;
}


void
roa_renderer_ctx_lock(
  roa_renderer_ctx_t ctx)
{
  ROA_UNUSED(ctx);
}


void
roa_renderer_ctx_unlock(
  roa_renderer_ctx_t ctx)
{
  ROA_UNUSED(ctx);
}

