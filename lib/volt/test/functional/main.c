
#include <GLFW/glfw3.h>
#include <stb/stb_image.h>
#include <volt/volt.h>
#include <roa_lib/fundamental.h>
#include <roa_lib/dir.h>
#include <roa_lib/dir.h>
#include <roa_lib/assert.h>
#include <roa_ctx/roa_ctx.h>
#include <roa_math/math.h>
#include <scratch/geometry.h>
#include <scratch/glsl.h>
#include <scratch/textures.h>
#include <string.h>


/* app stuff */
roa_ctx_t win_ctx;
void app_create();
void app_destroy();
ROA_BOOL app_tick();


int
main()
{
  app_create();

  volt_ctx_t ctx = VOLT_NULL;
  volt_ctx_create(&ctx);

  volt_vbo_t vbo                = VOLT_NULL;
  volt_ibo_t ibo                = VOLT_NULL;
  volt_texture_t texture_1      = VOLT_NULL;
  volt_texture_t texture_2      = VOLT_NULL;
  volt_program_t program        = VOLT_NULL;
  volt_input_t vert_input       = VOLT_NULL;
  volt_rasterizer_t rasterizer  = VOLT_NULL;
  volt_uniform_t uni_wvp        = VOLT_NULL;
  volt_sampler_t samp_1         = VOLT_NULL;
  volt_sampler_t samp_2         = VOLT_NULL;

  roa_mat4 world;
  roa_mat4 view;
  roa_mat4 proj;
  roa_mat4 wvp;

  /* create uniforms */
  {
    struct volt_uniform_desc uni_desc;
    ROA_MEM_ZERO(uni_desc);
    uni_desc.name       = "uni_wvp_mat";
    uni_desc.data_type  = VOLT_DATA_MAT4;
    uni_desc.count      = 1;

    volt_uniform_create(ctx, &uni_wvp, &uni_desc);
    ROA_ASSERT(uni_wvp);
  }

  /* create sampler */
  {
    struct volt_sampler_desc samp_desc_1;
    ROA_MEM_ZERO(samp_desc_1);
    samp_desc_1.name = "samp_diffuse_01";
    samp_desc_1.sampling = VOLT_SAMPLING_BILINEAR;

    volt_sampler_create(ctx, &samp_1, &samp_desc_1);

    struct volt_sampler_desc samp_desc_2;
    ROA_MEM_ZERO(samp_desc_2);
    samp_desc_2.name = "samp_diffuse_02";
    samp_desc_2.sampling = VOLT_SAMPLING_BILINEAR;

    volt_sampler_create(ctx, &samp_2, &samp_desc_2);
  }

  /* create some assets */
  {
    int x, y, n;

    /* textures */
    struct volt_texture_desc tex_desc_1;
    ROA_MEM_ZERO(tex_desc_1);

    tex_desc_1.dimentions = VOLT_TEXTURE_2D;
    tex_desc_1.mip_maps = VOLT_FALSE;
    tex_desc_1.sampling = VOLT_SAMPLING_BILINEAR;

    stbi_set_flip_vertically_on_load(1);

    unsigned bytes = 0;
    unsigned char *tex_data = texture_png_data_blender_1(&bytes);

    tex_desc_1.data = (void*)stbi_load_from_memory(
      tex_data,
      bytes,
      &x,
      &y,
      &n,
      0
    );

    tex_desc_1.width = x;
    tex_desc_1.height = y;
    tex_desc_1.format = n > 3 ? VOLT_PIXEL_FORMAT_RGBA : VOLT_PIXEL_FORMAT_RGB;

    struct volt_texture_desc tex_desc_2;
    ROA_MEM_ZERO(tex_desc_2);

    tex_desc_2.dimentions = VOLT_TEXTURE_2D;
    tex_desc_2.mip_maps = VOLT_FALSE;
    tex_desc_2.sampling = VOLT_SAMPLING_BILINEAR;

    tex_data = texture_png_data_blender_2(&bytes);

    tex_desc_2.data = (void*)stbi_load_from_memory(
      tex_data,
      bytes,
      &x,
      &y,
      &n,
      0
    );

    tex_desc_2.width = x;
    tex_desc_2.height = y;
    tex_desc_2.format = n > 3 ? VOLT_PIXEL_FORMAT_RGBA : VOLT_PIXEL_FORMAT_RGB;

    volt_texture_create(ctx, &texture_1, &tex_desc_1);
    volt_texture_create(ctx, &texture_2, &tex_desc_2);

    /* vbo */
    geom_vert_desc vert_desc[] = {
      GEOM_VERT_POSITION, GEOM_NORMAL, GEOM_UV,
    };

    float verts[1024];
    unsigned vert_count = 0;

    geometry_generate_cube(
      ROA_ARR_DATA(vert_desc),
      ROA_ARR_COUNT(vert_desc),
      0.5f,
      0.5f,
      0.5f,
      ROA_ARR_DATA(verts),
      &vert_count
    );

    struct volt_vbo_desc vbo_desc;
    ROA_MEM_ZERO(vbo_desc);

    vbo_desc.data = ROA_ARR_DATA(verts);
    vbo_desc.count = vert_count;

    volt_vertex_buffer_create(ctx, &vbo, &vbo_desc);

    /* ibo */
    unsigned index[] = {
      0,1,2,
      2,3,0
    };

    struct volt_ibo_desc ibo_desc;
    ROA_MEM_ZERO(ibo_desc);

    ibo_desc.data  = ROA_ARR_DATA(index);
    ibo_desc.count = ROA_ARR_COUNT(index);

    volt_index_buffer_create(ctx, &ibo, &ibo_desc);


    const char *stages[2];
    stages[0] = glsl_two_textures_vs();
    stages[1] = glsl_two_textures_fs();

    volt_shader_stage stage_types[] = {
      VOLT_SHD_VERTEX,
      VOLT_SHD_FRAGMENT,
    };

    struct volt_program_desc shd_desc;
    ROA_MEM_ZERO(shd_desc);

    shd_desc.shader_stages_src  = stages;
    shd_desc.shader_stages_type = stage_types;
    shd_desc.stage_count        = ROA_ARR_COUNT(stage_types);

    volt_program_create(ctx, &program, &shd_desc);

    volt_input_attribute input_fmt[] = {
      VOLT_INPUT_FLOAT3, /* pos */
      VOLT_INPUT_FLOAT3, /* color */
      VOLT_INPUT_FLOAT2, /* tex c */
    };

    struct volt_input_desc input_desc;
    ROA_MEM_ZERO(input_desc);

    input_desc.attributes = ROA_ARR_DATA(input_fmt);
    input_desc.count      = ROA_ARR_COUNT(input_fmt);

    volt_input_create(ctx, &vert_input, &input_desc);

    volt_ctx_execute(ctx);
  }

  /* app check */
  ROA_ASSERT(vbo);
  ROA_ASSERT(ibo);
  ROA_ASSERT(vert_input);
  ROA_ASSERT(program);
  ROA_ASSERT(texture_1);
  ROA_ASSERT(texture_2);

  while (app_tick())
  {
    struct roa_ctx_window_desc win_desc;
    roa_ctx_get_window_desc(win_ctx, &win_desc);

    static int count = 0;
    ++count;

    /* create mats */
    {
      static float time = 0.1f;
      time += 0.005f;
      float radius = 2.f;

      float aspect = (float)win_desc.width / (float)win_desc.height;
      roa_mat4_projection(&proj, ROA_QUART_TAU / 4, 0.1f, 10.f, aspect);

      float x = roa_float_sin(time) * radius;
      float y = radius - (radius / ROA_G_RATIO);
      float z = roa_float_cos(time) * radius;

      roa_float3 from = roa_float3_set_with_values(x, y, z);
      roa_float3 at   = roa_float3_fill_with_value(0.f);
      roa_float3 up   = roa_float3_set_with_values(0.f, 1.f, 0.f);

      roa_mat4_lookat(&view, from, at, up);

      roa_mat4_id(&world);

      roa_mat4_multiply_three(&wvp, &world, &view, &proj);
    }

    /* draw some stuff */
    {
      struct volt_renderpass_desc rp_desc;
      ROA_MEM_ZERO(rp_desc);
      rp_desc.name = "To Back Buffer";

      volt_renderpass_t rp;
      volt_renderpass_create(ctx, &rp, &rp_desc);

      struct volt_rect2d viewport = { 0,0,win_desc.width,win_desc.height };

      /* bind pipeline */
      {
        struct volt_pipeline_desc pipeline_desc;
        ROA_MEM_ZERO(pipeline_desc);
        pipeline_desc.viewport    = viewport;
        pipeline_desc.input       = vert_input;
        pipeline_desc.program     = program;
        pipeline_desc.rasterizer  = rasterizer;

        volt_renderpass_set_pipeline(rp, &pipeline_desc);
      }

      unsigned clear_flags = VOLT_CLEAR_COLOR | VOLT_CLEAR_DEPTH;
      volt_renderpass_clear_cmd(rp, clear_flags);

      /* bind buffers */
      {
        volt_renderpass_bind_texture_buffer_cmd(rp, samp_1, texture_1);
        volt_renderpass_bind_texture_buffer_cmd(rp, samp_2, texture_2);
      }

      /* uniforms */
      {
        volt_renderpass_bind_uniform_data_cmd(rp, uni_wvp, wvp.data);
      }

      /* draw */
      {
        struct volt_draw_desc draw_desc;
        ROA_MEM_ZERO(draw_desc);
        draw_desc.vbo = vbo;

        volt_renderpass_draw_cmd(rp, &draw_desc);
      }

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
  roa_ctx_create(&win_ctx);

  struct roa_ctx_window_desc win_desc;

  roa_ctx_get_window_desc(win_ctx, &win_desc);
  win_desc.title = "Volt Functional Test";
  win_desc.width = 1200;
  win_desc.height = 720;

  roa_ctx_set_window_desc(win_ctx, &win_desc);

  printf("OpenGL version is (%s)\n", glGetString(GL_VERSION));
}


ROA_BOOL
app_tick()
{
  return roa_ctx_new_frame(win_ctx);
}


void
app_destroy()
{
  roa_ctx_destroy(&win_ctx);
}
