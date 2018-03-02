#ifndef LOGN_DATA_INCLUDED_5F38BC59_37BE_47C4_B115_35915D54B865
#define LOGN_DATA_INCLUDED_5F38BC59_37BE_47C4_B115_35915D54B865


#include <volt/fwd.h>
#include <roa_ctx/roa_ctx.h>
#include <roa_math/math.h>
#include <roa_lib/alloc.h>


struct level_render_data
{
  volt_program_t program;
  volt_vbo_t vbo;
  volt_input_t vert_input;
  volt_rasterizer_t rasterizer;
  volt_uniform_t mvp[3];

  struct roa_tagged_allocator render_allocator;
};


struct door_data
{
  volt_ctx_t volt_ctx;
  roa_ctx_t win_ctx;

  roa_mat4 projection;
  roa_mat4 view;
  roa_mat4 world;

  struct level_render_data level_data;
};


#endif /* inc guard */
