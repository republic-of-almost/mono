#include <roa_ctx/roa_ctx.h>
#include <roa_lib/assert.h>
#include <roa_lib/alloc.h>
#include <roa_lib/fundamental.h>
#include <roa_job/roa_job.h>
#include <roa_math/math.h>
#include <volt/volt.h>
#include <roa_lib/time.h>


/* ---------------------------------------------------------- [ Systems ] -- */


roa_ctx_t                 hw_ctx;     /* windowing */
roa_job_dispatcher_ctx_t  job_ctx;    /* tasks */
volt_ctx_t                volt_ctx;   /* graphics api */


/* ------------------------------------------------------------ [ Scene ] -- */


volt_program_t    screen_program       = VOLT_NULL;
volt_texture_t    screen_texture       = VOLT_NULL;
volt_vbo_t        screen_triangle      = VOLT_NULL;
volt_input_t      screen_input_format  = VOLT_NULL;
volt_rasterizer_t screen_rasterizer    = VOLT_NULL;

int width = 1200;
int height = 720;

unsigned char *texture_data = ROA_NULL;


/* ------------------------------------------------------------ [ tasks ] -- */


struct worker_arg
{
  unsigned char *pix;
  unsigned length;
  unsigned offset;
  unsigned seed;
};


ROA_JOB(worker, struct worker_arg*)
{
  ROA_UNUSED(job_ctx);

  int i;
  for (i = 0; i < arg->length; ++i)
  {
    unsigned index = (arg->offset + i) * 4;
    unsigned rand_color = roa_rand_xorshift_with_seed(arg->seed * i * i + 2);

    arg->pix[index + 0] = ROA_FIRST8_BITS(rand_color);
    arg->pix[index + 1] = ROA_SECOND8_BITS(rand_color);
    arg->pix[index + 2] = ROA_THIRD8_BITS(rand_color);
    arg->pix[index + 3] = 255;
  }
}

ROA_JOB(main_frame, void*)
{
  ROA_UNUSED(arg);

  /* create worker tasks */
  struct volt_texture_desc tex_desc;
  {
    volt_texture_get_desc(volt_ctx, screen_texture, &tex_desc);

    struct roa_job_desc desc[128];
    struct worker_arg arg[ROA_ARR_COUNT(desc)];
    int i;

    static unsigned long time = 0;
    time += 1;

    for (i = 0; i < ROA_ARR_COUNT(desc); ++i)
    {
      unsigned length = (tex_desc.width * tex_desc.height) / ROA_ARR_COUNT(desc);

      arg[i].length = length;
      arg[i].offset = length * i;
      arg[i].pix = tex_desc.data;
      arg[i].seed = (unsigned)(time + i);

      desc[i].thread_locked = ROA_FALSE;
      desc[i].arg = &arg[i];
      desc[i].func = worker;
    }

    uint64_t marker = roa_job_submit(job_ctx, ROA_ARR_DATA(desc), ROA_ARR_COUNT(desc));
    roa_job_wait(job_ctx, marker);
  }

  /* update texture */
  {
    tex_desc.data = texture_data;
    volt_texture_update(volt_ctx, screen_texture, &tex_desc);
  }

  /* render scene */
  {
    volt_renderpass_t rp = ROA_NULL;
    volt_renderpass_create(volt_ctx, &rp, "draw triangle", ROA_NULL);

    volt_renderpass_bind_input_format(rp, screen_input_format);
    volt_renderpass_bind_vertex_buffer(rp, screen_triangle);
    volt_renderpass_bind_program(rp, screen_program);
    volt_renderpass_set_viewport(rp, 0, 0, width, height);
    volt_renderpass_bind_rasterizer(rp, screen_rasterizer);
    volt_renderpass_bind_texture_buffer(rp, screen_texture, "diffuse");
    volt_renderpass_draw(rp);

    volt_renderpass_commit(volt_ctx, &rp);
  }

  /* execute gpu commands */
  {
    volt_ctx_execute(volt_ctx);
  }

  /* submit new frame */
  if (roa_ctx_new_frame(hw_ctx))
  {
    struct roa_job_desc frame_desc;
    frame_desc.func           = main_frame;
    frame_desc.arg            = ROA_NULL;
    frame_desc.thread_locked  = ROA_TRUE;

    roa_job_submit(job_ctx, &frame_desc, 1);
  }
}


/* ------------------------------------------------------- [ Applicaton ] -- */


int
main(int argc, char **argv)
{
  /* command line args */
  ROA_UNUSED(argc);
  ROA_UNUSED(argv);

  /* create systems */
  {
    struct roa_ctx_window_desc win_desc;
    win_desc.width = width;
    win_desc.height = height;
    win_desc.title = "Rays";

    roa_ctx_create(&hw_ctx);
    roa_ctx_set_window_desc(hw_ctx, &win_desc);

    roa_job_dispatcher_ctx_create(&job_ctx, ROA_NULL);
    volt_ctx_create(&volt_ctx);
  }

  /* setup volt */
  {
    /* program */
    {
      volt_shader_stage stages[2];
      stages[0] = VOLT_SHD_VERTEX;
      stages[1] = VOLT_SHD_FRAGMENT;

      const char *vert_shd =
        "#version 150 core\n"
        "in vec2 vs_in_pos;\n"
        "in vec2 vs_in_tex_c;\n"
        "out vec2 ps_in_tex_c;\n"
        "void main()\n"
        "{\n"
        " gl_Position = vec4(vs_in_pos, 0.0, 1.0);\n"
        " ps_in_tex_c = vs_in_tex_c;\n"
        "}\n";

      const char *frag_shd =
        "#version 150 core\n"
        "in vec2 ps_in_tex_c;\n"
        "out vec4 out_color;\n"
        "uniform sampler2D diffuse;\n"
        "void main()\n"
        "{\n"
        " out_color = texture(diffuse, ps_in_tex_c);\n"
        "}\n";

      const char *shaders[2];
      shaders[0] = vert_shd;
      shaders[1] = frag_shd;
      
      struct volt_program_desc screen_program_desc;
      screen_program_desc.stage_count         = ROA_ARR_COUNT(stages);
      screen_program_desc.shader_stages_type  = ROA_ARR_DATA(stages);
      screen_program_desc.shader_stages_src   = ROA_ARR_DATA(shaders);

      volt_program_create(volt_ctx, &screen_program, &screen_program_desc);

      ROA_ASSERT(screen_program != VOLT_NULL);
			volt_ctx_execute(volt_ctx); /* because shader text will go out of scope */
    }

    /* texture */
    {
      texture_data = roa_alloc(width * height * 4);
      unsigned i;

      /* fill with random color for now */
      for (i = 0; i < width * height; ++i)
      {
        unsigned index = i * 4;
        texture_data[index + 0] = rand() % 255;
        texture_data[index + 1] = rand() % 255;
        texture_data[index + 2] = rand() % 255;
        texture_data[index + 3] = 255;
      }

      int tex_width = width;
      int tex_height = height;

      struct volt_texture_desc tex_desc;
      tex_desc.data       = ROA_ARR_DATA(texture_data);
      tex_desc.dimentions = VOLT_TEXTURE_2D;
      tex_desc.width      = tex_width;
      tex_desc.height     = tex_height;
      tex_desc.sampling   = VOLT_SAMPLING_BILINEAR;
      tex_desc.mip_maps   = VOLT_FALSE;
      tex_desc.format     = VOLT_COLOR_RGBA;
      tex_desc.access     = VOLT_STREAM;

      volt_texture_create(volt_ctx, &screen_texture, &tex_desc);

      ROA_ASSERT(screen_texture != VOLT_NULL);
    }

    /* triangle */
    {
      float verts[] = {
        /* x y, s t */
        +3.f, +1.f, 2.f, 0.f,
        -1.f, +1.f, 0.f, 0.f,
        -1.f, -3.f, 0.f, 2.f,
      };

      struct volt_vbo_desc vbo_desc;
      vbo_desc.data  = ROA_ARR_DATA(verts);
      vbo_desc.count = ROA_ARR_COUNT(verts);

      volt_vertex_buffer_create(volt_ctx, &screen_triangle, &vbo_desc);

      ROA_ASSERT(screen_triangle != VOLT_NULL);
			volt_ctx_execute(volt_ctx);
    }

    /* input */
    {
      volt_input_attribute attrs[2];
      attrs[0] = VOLT_INPUT_FLOAT2;
      attrs[1] = VOLT_INPUT_FLOAT2;

      struct volt_input_desc input_desc;
      input_desc.attributes = ROA_ARR_DATA(attrs);
      input_desc.count      = ROA_ARR_COUNT(attrs);

      /*volt_input_t   input_format;*/
      volt_input_create(volt_ctx, &screen_input_format, &input_desc);

      ROA_ASSERT(screen_input_format != VOLT_NULL);
			volt_ctx_execute(volt_ctx); /* attrs will go out of scope */
    }

    /* rasterizer */
    {
      struct volt_rasterizer_desc raster_desc;
      raster_desc.cull_mode = VOLT_CULL_FRONT;
      raster_desc.primitive_type = VOLT_PRIM_TRIANGLES;
      raster_desc.winding_order = VOLT_WIND_CW;

      volt_rasterizer_create(volt_ctx, &screen_rasterizer, &raster_desc);
    }

    volt_ctx_execute(volt_ctx);
  }

  /* start dispatcher */
  {
    struct roa_job_desc frame_desc;
    frame_desc.func           = main_frame;
    frame_desc.arg            = ROA_NULL;
    frame_desc.thread_locked  = ROA_TRUE;

    roa_job_submit(job_ctx, &frame_desc, 1);
    roa_job_dispatcher_ctx_run(job_ctx);
  }

  /* destroy systems */
  {
    volt_ctx_destroy(&volt_ctx);
    roa_job_dispatcher_ctx_destroy(&job_ctx);
    roa_ctx_destroy(&hw_ctx);
  }

  return 0;
}
