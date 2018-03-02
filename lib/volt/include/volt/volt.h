#ifndef VOLT_INCLUDED_F1E11FF6_C036_4211_A608_BA0DC5DA0259
#define VOLT_INCLUDED_F1E11FF6_C036_4211_A608_BA0DC5DA0259


#include <volt/fwd.h>


#ifdef __cplusplus
extern "C" {
#endif


 /* ----------------------------------------------------------- [ types ] -- */


#define VOLT_NULL 0

typedef int VOLT_BOOL;

#define VOLT_TRUE 1
#define VOLT_FALSE 0


typedef enum _volt_color_format {
  VOLT_COLOR_RGB,
  VOLT_COLOR_RGBA
} volt_color_format;


typedef enum _volt_texture_dimentions {
  VOLT_TEXTURE_2D,
} volt_texture_dimentions;


typedef enum _volt_texture_sampling {
  VOLT_SAMPLING_BILINEAR,
} volt_texture_sampling;


typedef enum _volt_input_attribute {
  VOLT_INPUT_FLOAT4,
  VOLT_INPUT_FLOAT3,
  VOLT_INPUT_FLOAT2,
  VOLT_INPUT_FLOAT,

} volt_input_attribute;


typedef enum _volt_shader_stage {
  VOLT_SHD_VERTEX,
  VOLT_SHD_GEOMETRY,
  VOLT_SHD_FRAGMENT,
  VOLT_SHD_STAGE_COUNT,
} volt_shader_stage;



/* --------------------------------------------------------- [ lifetime ] -- */


void
volt_ctx_create(volt_ctx_t *ctx);


void
volt_ctx_destroy(volt_ctx_t *ctx);


void
volt_ctx_execute(volt_ctx_t ctx);


typedef void(*volt_log_callback_fn)(const char *msg);


void
volt_ctx_logging_callback(volt_ctx_t ctx, volt_log_callback_fn callback);


/* -------------------------------------------------------- [ resources ] -- */


typedef enum _volt_resource_status {
  VOLT_RSRC_PENDING_CREATE,
  VOLT_RSRC_FAILED,
  VOLT_RSRC_VALID,
  VOLT_RSRC_PENDING_DESTROY,
} volt_resource_status;


/* ----------------------------------------------------- [ rsrc texture ] -- */


struct volt_texture_desc
{
  volt_texture_dimentions dimentions;
  volt_texture_sampling sampling;
  volt_color_format format;
  VOLT_BOOL mip_maps;

  void *data;
  unsigned width;
  unsigned height;
};


void
volt_texture_create(
  volt_ctx_t ctx,
  volt_texture_t *texture,
  struct volt_texture_desc *desc);


/* ------------------------------------------------- [ rsrc framebuffer ] -- */


struct volt_framebuffer_desc
{
  volt_texture_t attachment;
};


void
volt_frame_buffer_create(
  volt_ctx_t ctx,
  volt_framebuffer_t *fbo,
  struct volt_framebuffer_desc *desc);


/* ------------------------------------------------------- [ rsrc input ] -- */


struct volt_input_desc
{
  volt_input_attribute *attributes;
  unsigned count;
};


void
volt_input_create(
  volt_ctx_t ctx,
  volt_input_t *input,
  struct volt_input_desc *desc);


/* --------------------------------------------------------- [ rsrc vbo ] -- */


struct volt_vbo_desc
{
  float *data;
  unsigned count;
};


void
volt_vertex_buffer_create(
  volt_ctx_t ctx,
  volt_vbo_t *t,
  struct volt_vbo_desc *desc);


/* --------------------------------------------------------- [ rsrc ibo ] -- */


struct volt_ibo_desc
{
  unsigned int *data;
  unsigned count;
};


void
volt_index_buffer_create(
  volt_ctx_t ctx,
  volt_ibo_t *t,
  struct volt_ibo_desc *desc);


/* ----------------------------------------------------- [ rsrc program ] -- */


struct volt_program_desc
{
  const char **shader_stages_src;
  volt_shader_stage *shader_stages_type;

  unsigned stage_count;
};


void
volt_program_create(
  volt_ctx_t ctx,
  volt_program_t *program,
  struct volt_program_desc *desc);


/* ----------------------------------------------------- [ rsrc uniform ] -- */


typedef enum _volt_data_type {
  VOLT_DATA_FLOAT,
  VOLT_DATA_FLOAT2,
  VOLT_DATA_FLOAT3,
  VOLT_DATA_FLOAT4,
  VOLT_DATA_MAT2,
  VOLT_DATA_MAT3,
  VOLT_DATA_MAT4,
} volt_data_type;


struct volt_uniform_desc
{
  volt_data_type data_type;
  unsigned count;
};


void
volt_uniform_create(
  volt_ctx_t ctx,
  volt_uniform_t *uniform,
  struct volt_uniform_desc *desc);


void
volt_uniform_update(
  volt_ctx_t ctx,
  volt_uniform_t uniform,
  void *data);


/* -------------------------------------------------- [ rsrc rasterizer ] -- */


typedef enum _volt_primitive_type {
  VOLT_PRIM_TRIANGLES,
  VOLT_PRIM_POINT,
} volt_primitive_type;


typedef enum _volt_winding_order {
  VOLT_WIND_CCW,
  VOLT_WIND_CW,
} volt_winding_order;


typedef enum _volt_cull_mode {
  VOLT_CULL_BACK,
  VOLT_CULL_FRONT,
} volt_cull_mode;


struct volt_rasterizer_desc
{
  volt_primitive_type primitive_type;
  volt_winding_order winding_order;
  volt_cull_mode cull_mode;
};


void
volt_rasterizer_create(
  volt_ctx_t ctx,
  volt_rasterizer_t *rasterizer,
  struct volt_rasterizer_desc *desc);


/* ------------------------------------------------------- [ renderpass ] -- */


void
volt_renderpass_create(
  volt_ctx_t ctx,
  volt_renderpass_t *pass,
  const char *pass_name,
  volt_framebuffer_t target);


void
volt_renderpass_commit(
  volt_ctx_t ctx,
  volt_renderpass_t *pass);


void
volt_renderpass_bind_rasterizer(
  volt_renderpass_t pass,
  volt_rasterizer_t rasterizer);


void
volt_renderpass_bind_input_format(
  volt_renderpass_t pass,
  volt_input_t);


void
volt_renderpass_bind_vertex_buffer(
  volt_renderpass_t pass,
  volt_vbo_t vbo);


void
volt_renderpass_bind_index_buffer(
  volt_renderpass_t pass,
  volt_ibo_t ibo);


void
volt_renderpass_bind_texture_buffer(
  volt_renderpass_t pass,
  volt_texture_t texture,
  const char *location);


void
volt_renderpass_bind_program(
  volt_renderpass_t pass,
  volt_program_t program);


void
volt_renderpass_bind_uniform(
  volt_renderpass_t pass,
  volt_uniform_t uniform,
  const char *location);


void
volt_renderpass_set_viewport(
  volt_renderpass_t pass,
  int x,
  int y,
  unsigned width,
  unsigned height);


void
volt_renderpass_draw(
  volt_renderpass_t pass);


#ifdef __cplusplus
} /* extern */
#endif


#endif /* inc guard */