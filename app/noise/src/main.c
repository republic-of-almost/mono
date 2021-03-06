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

#define SCREEN_WIDTH 200
#define SCREEN_HEIGHT 200

unsigned char *texture_data = ROA_NULL;


/* ------------------------------------------------------------ [ tasks ] -- */


float random(roa_float2 _st)
{
	float dot = roa_float2_dot(_st, roa_float2_set_with_values(12.9898f, 78.233f));
	float sin = roa_float_sin(dot) * 43758.5453123f;
	float fract = roa_float_fract(sin);

	/*
	printf("floor %f : %f\n", -0.234, roa_float_floor(-1.234));
	printf("fract %f : %f\n", sin, fract);
	*/

	return fract;
}


float noise(roa_float2 _st)
{

  roa_float2 i = roa_float2_floor(_st);
  roa_float2 f = roa_float2_fract(_st);


  /* Four corners in 2D of a tile */
  float a = random(roa_float2_add(i, roa_float2_set_with_values(0.f, 0.f)));
  float b = random(roa_float2_add(i, roa_float2_set_with_values(1.f, 0.f)));
  float c = random(roa_float2_add(i, roa_float2_set_with_values(0.f, 1.f)));
  float d = random(roa_float2_add(i, roa_float2_set_with_values(1.f, 1.f)));
	
	roa_float2 ff = roa_float2_multiply(f, f);
	roa_float2 f2 = roa_float2_scale(f, 2.f);
	roa_float2 three = roa_float2_fill_with_value(3.f);
	roa_float2 three_neg_f2 = roa_float2_subtract(three, f2);
	roa_float2 u = roa_float2_multiply(ff, three_neg_f2);


  
 return roa_float_lerp(a, b, u.x) +
    (c - a)* u.y * (1.0 - u.x) +
    (d - b) * u.x * u.y;

	/*
	float value = 0.f;
	value += f.x * f.y * a;
	value += (1 - f.x) * f.y * b;
	value += f.x * (1 + f.y) * c;
	value += (1 - f.x) * (1 + f.y) * d;

	return value;
	*/
}

#define NUM_OCTAVES 7 

float fbm(roa_float2 _st) {
  float v = 0.0;
  float a = 0.5f;

  roa_float2 shift = roa_float2_fill_with_value(100.0);
	
  /* Rotate to reduce axial bias */

  float mat_data[4] = {
    roa_float_cos(0.5), roa_float_sin(0.5),
    -roa_float_sin(0.5), roa_float_cos(0.50)
  };

  roa_mat2 rot;
  roa_mat2_import(&rot, mat_data);
	
	int i;
  for (i = 0; i < NUM_OCTAVES; ++i)
  {
    v += a * noise(_st);
    /*_st = roa_mat2_multiply_with_float2(roa_float2_add(roa_float2_scale(_st, 2.0), shift), &rot);*/
		_st = roa_float2_scale(_st, 2.f);
    a *= 0.5;
  }
  return v; 
}


float smooth_noise(roa_float2 st)
{
	return 0.f;
}


struct worker_arg
{
  unsigned char *pix;
  unsigned length;
  unsigned offset;
  float seed;
  unsigned row;
};


ROA_JOB(worker, struct worker_arg*)
{
  ROA_UNUSED(job_ctx);

  /*roa_float2 res_3 = roa_float2_set_with_values(width, height); */
  /*res_3 = roa_float2_scale(res_3, 3.f); */


  int i;
  for (i = 0; i < arg->length; ++i)
  {
    roa_float2 frag_coord = roa_float2_set_with_values((float)i, (float)arg->row);

    roa_float2 st = roa_float2_divide(frag_coord, roa_float2_set_with_values(SCREEN_WIDTH, SCREEN_HEIGHT));
		roa_float2 scaled_st = roa_float2_scale(st, 7.f);

		/* q and r elements */
		roa_float2 q = roa_float2_zero();
		q.x = fbm(scaled_st);
		q.y = fbm(roa_float2_add(q, roa_float2_one()));


		roa_float2 r = roa_float2_zero();
		roa_float2 rx_time = roa_float2_fill_with_value(0.15 * arg->seed);
		r.x = fbm(roa_float2_add(st, rx_time));
		

		float f = fbm(roa_float2_add(scaled_st, r));

		
		/* color */
		float init_time = roa_float_clamp((f * f) * 4.f, 0.f, 1.f);
		roa_float3 start_col = roa_float3_set_with_values(0.1019f, 0.6196f, 0.6667f);
		roa_float3 end_col = roa_float3_set_with_values(0.6667f, 0.6667f, 0.4980f);
		roa_float3 color = roa_float3_lerp(start_col, end_col, init_time);


		/* final color */
		float color_scale = f * f * f + 0.6f * f * f + 0.5f * f;
		color = roa_float3_scale(color, color_scale);

		roa_float3 final_color = color;
		final_color = roa_float3_scale(final_color, 200);

		/* apply color */
    unsigned index = (arg->offset + i) * 4;

    arg->pix[index + 0] = final_color.x;
    arg->pix[index + 1] = final_color.y;
    arg->pix[index + 2] = final_color.z;
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

    struct roa_job_desc desc[SCREEN_WIDTH];
    struct worker_arg arg[ROA_ARR_COUNT(desc)];
    int i;

    static float time = 0;
    time += 0.1f;

    for (i = 0; i < ROA_ARR_COUNT(desc); ++i)
    {
      unsigned length = (tex_desc.width * tex_desc.height) / ROA_ARR_COUNT(desc);

      arg[i].length = length;
      arg[i].offset = length * i;
      arg[i].pix = tex_desc.data;
      arg[i].seed = time; /*(unsigned)(time + i);*/
      arg[i].row = i;

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
    volt_renderpass_set_viewport(rp, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
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
    win_desc.width = SCREEN_WIDTH;
    win_desc.height = SCREEN_HEIGHT;
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
      texture_data = roa_alloc(SCREEN_WIDTH * SCREEN_HEIGHT * 4);
      unsigned i;

      /* fill with random color for now */
      for (i = 0; i < SCREEN_WIDTH * SCREEN_HEIGHT; ++i)
      {
        unsigned index = i * 4;
        texture_data[index + 0] = rand() % 255;
        texture_data[index + 1] = rand() % 255;
        texture_data[index + 2] = rand() % 255;
        texture_data[index + 3] = 255;
      }

      int tex_width = SCREEN_WIDTH;
      int tex_height = SCREEN_HEIGHT;

      struct volt_texture_desc tex_desc;
      tex_desc.data       = ROA_ARR_DATA(texture_data);
      tex_desc.dimentions = VOLT_TEXTURE_2D;
      tex_desc.width      = tex_width;
      tex_desc.height     = tex_height;
      tex_desc.sampling   = VOLT_SAMPLING_BILINEAR;
      tex_desc.mip_maps   = VOLT_FALSE;
      tex_desc.format     = VOLT_PIXEL_FORMAT_RGBA;
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
      raster_desc.primitive = VOLT_PRIM_TRIANGLES;
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
