#include <doors/door_renderer/door_renderer.h>
#include <doors/door_data/door_data.h>
#include <roa_lib/fundamental.h>
#include <common/app_data.h>
#include <roa_ctx/roa_ctx.h>
#include <roa_lib/assert.h>
#include <volt/volt.h>
#include <volt/utils/prim_model.h>


/* ----------------------------------------------------- [ Render Level ] -- */


ROA_JOB(door_render, struct door_data*)
{ 
  VOLT_BOOL pending = volt_ctx_has_pending_rsrcs(arg->volt_ctx);

  if(!pending)
  {
    /* calculate the things to needed to render */
    
    struct roa_ctx_window_desc win_desc;
    roa_ctx_get_window_desc(arg->win_ctx, &win_desc);

    ROA_ASSERT(arg->level_data.program);
  
    struct level_render_data *level_data = &arg->level_data;

    /* update matrix */
    {
      static float time = 0.1f;
      time += 0.01f;
      float radius = 3.f;

      float aspect = (float)win_desc.width / (float)win_desc.height;

      roa_mat4_projection(&arg->projection, ROA_QUART_TAU * 0.25, 0.1f, 10.f, aspect);

      float x = roa_sin(time) * radius;
      float y = roa_cos(time) * radius;
      float z = radius - (radius / ROA_G_RATIO);

      roa_float3 from = roa_float3_set_with_values(x, y, z);
      roa_float3 at = roa_float3_fill_with_value(0.f);
      roa_float3 up = roa_float3_set_with_values(0.f, 0.f, 1.f);

      roa_mat4_lookat(&arg->view, from, at, up);

      roa_mat4_id(&arg->world);

      volt_uniform_update(arg->volt_ctx, arg->level_data.mvp[1], arg->view.data);
      volt_uniform_update(arg->volt_ctx, arg->level_data.mvp[2], arg->projection.data);
      volt_uniform_update(arg->volt_ctx, arg->level_data.mvp[0], arg->world.data);
    }

    /* renderpass */
    {
      volt_renderpass_t rp = VOLT_NULL;
      volt_renderpass_create(arg->volt_ctx, &rp, ROA_NULL, ROA_NULL);

      volt_renderpass_set_viewport(rp, 0,0, win_desc.width, win_desc.height);

      volt_renderpass_bind_program(rp, level_data->program);

      volt_renderpass_bind_input_format(rp, level_data->vert_input);
      volt_renderpass_bind_vertex_buffer(rp, level_data->vbo);
      
      volt_renderpass_bind_uniform(rp, arg->level_data.mvp[2], "proj");
      volt_renderpass_bind_uniform(rp, arg->level_data.mvp[1], "view");
      volt_renderpass_bind_uniform(rp, arg->level_data.mvp[0], "model");

      volt_renderpass_draw(rp);
      volt_renderpass_commit(arg->volt_ctx, &rp);
    }
  }
}


/* -------------------------------------------- [ Create Rendering Data ] -- */


ROA_JOB(door_renderer_create, struct door_data*)
{
  /* program */
  {
    const char vert_src[] = ""
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

    int sizeof_vert_src = sizeof(vert_src);

    char *data_vert_src = roa_tagged_allocator_copy(
      &arg->level_data.render_allocator,
      vert_src,
      sizeof(vert_src) + 1);
    

    const char frag_src[] = ""
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

    char *data_frag_src = roa_tagged_allocator_copy(
      &arg->level_data.render_allocator,
      frag_src,
      sizeof(frag_src) + 1);

    const char *stages[2];
    stages[0] = data_vert_src;
    stages[1] = data_frag_src;

    char *data_stages = roa_tagged_allocator_copy(
      &arg->level_data.render_allocator,
      stages,
      sizeof(stages));

    volt_shader_stage stage_types[2];
    stage_types[0] = VOLT_SHD_VERTEX;
    stage_types[1] = VOLT_SHD_FRAGMENT;

    volt_shader_stage *data_stage_types = roa_tagged_allocator_copy(
      &arg->level_data.render_allocator,
      stage_types,
      sizeof(stage_types));

    struct volt_program_desc shd_desc;
    shd_desc.shader_stages_src = data_stages;
    shd_desc.shader_stages_type = data_stage_types;
    shd_desc.stage_count = 2;

    volt_program_create(
      arg->volt_ctx, 
      &arg->level_data.program,
      &shd_desc);
  }

  /* input */
  {
    volt_input_attribute input_fmt[] = {
      VOLT_INPUT_FLOAT3, /* pos */
      VOLT_INPUT_FLOAT3, /* normal */
      VOLT_INPUT_FLOAT2, /* tex c */
    };

    volt_input_attribute *data_input_fmt = roa_tagged_allocator_copy(
      &arg->level_data.render_allocator,
      input_fmt,
      sizeof(input_fmt));

    struct volt_input_desc input_desc;
    input_desc.attributes = data_input_fmt;
    input_desc.count = ROA_ARR_COUNT(input_fmt);

    volt_input_create(
      arg->volt_ctx,
      &arg->level_data.vert_input,
      &input_desc);
  }

  /* vbo */
  {
    volt_vert_desc vert_desc[] = {
      VOLT_VERT_POSITION,
      VOLT_NORMAL,
      VOLT_UV,
    };
    unsigned vert_count = 0;

    volt_util_generate_cube(
      ROA_NULL,
      ROA_NULL,
      1.f,
      1.f,
      1.f,
      ROA_NULL,
      &vert_count
    );

    float *data_verts = roa_tagged_allocator_alloc(
      &arg->level_data.render_allocator,
      sizeof(float) * vert_count);

    volt_util_generate_cube(
      ROA_ARR_DATA(vert_desc),
      ROA_ARR_COUNT(vert_desc),
      1.f,
      1.f,
      1.f,
      data_verts,
      &vert_count
    );

    struct volt_vbo_desc vbo_desc;
    vbo_desc.data = data_verts;
    vbo_desc.count = vert_count;

    volt_vertex_buffer_create(
      arg->volt_ctx,
      &arg->level_data.vbo,
      &vbo_desc);
  }

  /* create uniforms */
  {
    struct volt_uniform_desc uni_desc;
    uni_desc.data_type = VOLT_DATA_MAT4;
    uni_desc.count = 1;

    volt_uniform_create(arg->volt_ctx, &arg->level_data.mvp[2], &uni_desc);
    volt_uniform_create(arg->volt_ctx, &arg->level_data.mvp[1], &uni_desc);
    volt_uniform_create(arg->volt_ctx, &arg->level_data.mvp[0], &uni_desc);
  }
}