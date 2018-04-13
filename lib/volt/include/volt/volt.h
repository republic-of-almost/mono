#ifndef VOLT_INCLUDED_F1E11FF6_C036_4211_A608_BA0DC5DA0259
#define VOLT_INCLUDED_F1E11FF6_C036_4211_A608_BA0DC5DA0259


#include <volt/fwd.h>


#ifdef __cplusplus
extern "C" {
#endif


 /* ----------------------------------------------------------- [ types ] -- */


#define VOLT_NULL 0

typedef int volt_bool;

#define VOLT_TRUE 1
#define VOLT_FALSE 0


struct volt_rect2d
{
  int x
  int y;
  unsigned width;
  unsigned height;
};


typedef enum _volt_pixel_format {
  VOLT_PIXEL_FORMAT_RGB,
  VOLT_PIXEL_FORMAT_RGBA,
  VOLT_PIXEL_FORMAT_RGBA32F,

  VOLT_PIXEL_FORMAT_DEPTH32F,
  VOLT_PIXEL_FORMAT_DEPTH32F_STENCIL8,
} volt_pixel_format;


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


typedef enum _volt_access {
  VOLT_STREAM,
  VOLT_STATIC,
} volt_access;


typedef enum _volt_clear {
  VOLT_CLEAR_COLOR = 1 << 0,
  VOLT_CLEAR_DEPTH = 1 << 1,
  VOLT_CLEAR_STENCIL = 1 << 2,
} volt_clear;

/* --------------------------------------------------------- [ lifetime ] -- */


void
volt_ctx_create(volt_ctx_t *ctx);


void
volt_ctx_destroy(volt_ctx_t *ctx);


void
volt_ctx_execute(volt_ctx_t ctx);


volt_bool
volt_ctx_has_pending_rsrcs(volt_ctx_t ctx);


typedef void(*volt_log_callback_fn)(const char *msg);


void
volt_ctx_logging_callback(volt_ctx_t ctx, volt_log_callback_fn callback);


/* -------------------------------------------------------- [ resources ] -- */


typedef enum _volt_resource_status {
  VOLT_RSRC_PENDING_CREATE,
  VOLT_RSRC_ERROR,
  VOLT_RSRC_VALID,
  VOLT_RSRC_PENDING_DESTROY,
} volt_resource_status;


/* ----------------------------------------------------- [ rsrc texture ] -- */


struct volt_texture_desc
{
  volt_texture_dimentions dimentions;
  volt_texture_sampling sampling;
  volt_pixel_format format;
  volt_bool mip_maps;
  volt_access access;

  const char *name;

  void *data;
  unsigned width;
  unsigned height;
};


void
volt_texture_create(
  volt_ctx_t ctx,
  volt_texture_t *texture,
  struct volt_texture_desc *desc);


void
volt_texture_update(
  volt_ctx_t ctx,
  volt_texture_t texture,
  struct volt_texture_desc *desc);


void
volt_texture_get_desc(
  volt_ctx_t ctx,
  volt_texture_t texture,
  struct volt_texture_desc *out_desc);


/* ------------------------------------------------- [ rsrc framebuffer ] -- */


struct volt_framebuffer_desc
{
  volt_texture_t *attachments;
  unsigned attachment_count;

  volt_texture_t depth;
};


void
volt_framebuffer_create(
  volt_ctx_t ctx,
  volt_framebuffer_t *fbo,
  struct volt_framebuffer_desc *desc);


volt_resource_status
volt_framebuffer_status(
  volt_ctx_t ctx,
  volt_framebuffer_t);


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


volt_resource_status
volt_input_status(
  volt_ctx_t ctx,
  volt_input_t input);


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


volt_resource_status
volt_vertex_buffer_status(
  volt_ctx_t ctx,
  volt_vbo_t vbo);


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


volt_resource_status
volt_index_buffer_status(
  volt_ctx_t ctx,
  volt_ibo_t ibo);


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


volt_resource_status
volt_program_buffer_status(
  volt_ctx_t ctx,
  volt_program_t program);


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


struct volt_renderpass_desc
{
  volt_framebuffer_t  fbo;                /* optional - fbo to render to - VOLT_NULL if default backbuffer. */
  unsigned *          attachments;        /* optional - if fbo bound, and attachments VOLT_NULL then defaults to all color attachments. */
  unsigned            attachment_count;   /* number of attachments. */
  const char *        name;               /* optional - if supported used in debugging. */
  volt_bool           copy_uniform_data;  /* copies uniform into internal buffer. */
};


void
volt_renderpass_create(
  volt_ctx_t                    ctx,      /* valid volt ctx */
  volt_renderpass_t *           pass,     /* out pass value */
  struct volt_renderpass_desc * desc);    /* renderpass desc */


void
volt_renderpass_commit(
  volt_ctx_t                    ctx,      /* valid volt ctx */
  volt_renderpass_t *           pass);    /* renderpass to submit - invalid after submission */


void
volt_renderpass_bind_rasterizer(
  volt_renderpass_t             pass,         /* valid renderpass */
  volt_rasterizer_t             rasterizer);  /* rasterizer object */


void
volt_renderpass_bind_input_format(
  volt_renderpass_t             pass,     /* valid renderpass */
  volt_input_t                  input);   /* valid input format */


void
volt_renderpass_bind_vertex_buffer(
  volt_renderpass_t             pass,     /* valid renderpass */
  volt_vbo_t                    vbo);     /* valid vbo */


void
volt_renderpass_bind_index_buffer(
  volt_renderpass_t             pass,     /* valid renderpass */
  volt_ibo_t                    ibo);     /* valid ibo */


void
volt_renderpass_bind_texture_buffer(
  volt_renderpass_t             pass,     /* valid renderpass */
  volt_texture_t                texture,  /* valid texture */
  const char *                  location);/* sampler to bind to */


void
volt_renderpass_bind_program(
  volt_renderpass_t             pass,     /* valid renderpass */
  volt_program_t                program); /* valid program */


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
volt_renderpass_set_scissor(
  volt_renderpass_t pass,
  int x,
  int y,
  unsigned width,
  unsigned height);


void
volt_renderpass_clear(
  volt_renderpass_t pass,
  unsigned volt_clear_flags);


void
volt_renderpass_draw(
  volt_renderpass_t pass);


#ifdef __cplusplus
} /* extern */
#endif


#endif /* inc guard */
