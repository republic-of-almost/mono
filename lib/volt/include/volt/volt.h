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
  int x;
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
  VOLT_CLEAR_COLOR    = 1 << 0,
  VOLT_CLEAR_DEPTH    = 1 << 1,
  VOLT_CLEAR_STENCIL  = 1 << 2,
} volt_clear;


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


typedef enum _volt_data_type {
  VOLT_DATA_FLOAT,
  VOLT_DATA_FLOAT2,
  VOLT_DATA_FLOAT3,
  VOLT_DATA_FLOAT4,
  VOLT_DATA_INT,
  VOLT_DATA_INT2,
  VOLT_DATA_INT3,
  VOLT_DATA_INT4,
  VOLT_DATA_MAT2,
  VOLT_DATA_MAT3,
  VOLT_DATA_MAT4,
} volt_data_type;


typedef enum _volt_resource_status {
  VOLT_RSRC_PENDING_CREATE,
  VOLT_RSRC_ERROR,
  VOLT_RSRC_VALID,
  VOLT_RSRC_PENDING_DESTROY,
} volt_resource_status;


/* --------------------------------------------------------- [ lifetime ] -- */
/*
  Lifetime of the context and application, provide an error callback for error
  handling information.

  volt_ctx_execute() must be called from the thread that volt_ctx_create was
  called.
*/


void
volt_ctx_create(
  volt_ctx_t *ctx);


void
volt_ctx_destroy(
  volt_ctx_t *ctx);


void
volt_ctx_execute(
  volt_ctx_t ctx);


volt_bool
volt_ctx_has_pending_rsrcs(
  volt_ctx_t ctx);


typedef void(*volt_log_callback_fn)(const char *msg);


void
volt_ctx_logging_callback(
  volt_ctx_t ctx,
  volt_log_callback_fn callback);


/* ----------------------------------------------------- [ rsrc texture ] -- */
/*
  Texture
  --
  Texture holds data that can be written / read from in the program.
*/


struct volt_texture_desc
{
  volt_texture_dimentions dimentions;
  volt_texture_sampling   sampling;
  volt_pixel_format       format;
  volt_bool               mip_maps;
  volt_access             access;
  const char *            name;
  void *                  data;
  unsigned                width;
  unsigned                height;
};


void
volt_texture_create(
  volt_ctx_t                  ctx,
  volt_texture_t *            texture,
  struct volt_texture_desc *  desc);


void
volt_texture_update(
  volt_ctx_t                  ctx,
  volt_texture_t              texture,
  struct volt_texture_desc *  desc);


void
volt_texture_get_desc(
  volt_ctx_t                  ctx,
  volt_texture_t              texture,
  struct volt_texture_desc *  out_desc);


/* ------------------------------------------------- [ rsrc framebuffer ] -- */
/*
  FBO
  --
  A frame buffer is a texture (or collection of textures) which can be writen
  to during a renderpass.
*/


struct volt_framebuffer_desc
{
  volt_texture_t *    attachments;
  unsigned            attachment_count;
  volt_texture_t      depth;
};


void
volt_framebuffer_create(
  volt_ctx_t                      ctx,
  volt_framebuffer_t *            fbo,
  struct volt_framebuffer_desc *  desc);


volt_resource_status
volt_framebuffer_status(
  volt_ctx_t                      ctx,
  volt_framebuffer_t              fbo);


/* ------------------------------------------------------- [ rsrc input ] -- */
/*
  Input
  --
  Describes the vertex format of a vbo during a render.
*/


struct volt_input_desc
{
  volt_input_attribute *  attributes;
  unsigned                count;
};


void
volt_input_create(
  volt_ctx_t                ctx,
  volt_input_t *            input,
  struct volt_input_desc *  desc);


volt_resource_status
volt_input_status(
  volt_ctx_t                ctx,
  volt_input_t              input);


/* --------------------------------------------------------- [ rsrc vbo ] -- */
/*
  VBO
  --
  A vertex buffer represents geometry that gets sent to the GPU.
*/


struct volt_vbo_desc
{
  float *   data;
  unsigned  count;
};


void
volt_vertex_buffer_create(
  volt_ctx_t              ctx,
  volt_vbo_t *            vbo,
  struct volt_vbo_desc *  desc);


volt_resource_status
volt_vertex_buffer_status(
  volt_ctx_t              ctx,
  volt_vbo_t              vbo);


/* --------------------------------------------------------- [ rsrc ibo ] -- */
/*
  IBO
  --
  An index buffer, will index a vbo during a renderpass.
*/


struct volt_ibo_desc
{
  unsigned int *    data;
  unsigned          count;
};


void
volt_index_buffer_create(
  volt_ctx_t              ctx,
  volt_ibo_t *            ibo,
  struct volt_ibo_desc *  desc);


volt_resource_status
volt_index_buffer_status(
  volt_ctx_t              ctx,
  volt_ibo_t              ibo);


/* ----------------------------------------------------- [ rsrc program ] -- */
/*
  Program
  --
  A program represents the code that will execute on the GPU.
*/


struct volt_program_desc
{
  const char **       shader_stages_src;
  volt_shader_stage * shader_stages_type;
  unsigned            stage_count;
};


void
volt_program_create(
  volt_ctx_t                  ctx,
  volt_program_t *            program,
  struct volt_program_desc *  desc);


volt_resource_status
volt_program_buffer_status(
  volt_ctx_t                  ctx,
  volt_program_t              program);


/* ----------------------------------------------------- [ rsrc uniform ] -- */
/*
  Uniform
  --
  Used to bind data to a shader during a renderpass.
*/


struct volt_uniform_desc
{
  const char *      name;
  volt_data_type    data_type;
  volt_bool         copy_data;
  unsigned          count;
};


void
volt_uniform_create(
  volt_ctx_t                    ctx,
  volt_uniform_t *              uniform,
  struct volt_uniform_desc *    desc);


void
volt_uniform_update(
  volt_ctx_t                    ctx,
  volt_uniform_t                uniform,
  void *                        data);


/* ----------------------------------------------------- [ rsrc sampler ] -- */
/*
  Sampler
  --
  Used to bind a texture buffer to a shader during a renderpass.
*/


struct volt_sampler_desc
{
  const char *            name;             /* name of sampler */
  volt_texture_sampling   sampling;         /* sampling method */
};


void
volt_sampler_create(
  volt_ctx_t                      ctx,      /* valid volt ctx */
  volt_sampler_t *                sampler,  /* out sampler */
  struct volt_sampler_desc *      desc);    /* sampler desc */



/* -------------------------------------------------- [ rsrc rasterizer ] -- */
/*
  Rasterizer
  --
  Describes how the triangles are rendered in a renderpass.
*/


struct volt_rasterizer_desc
{
  volt_primitive_type   primitive_type;
  volt_winding_order    winding_order;
  volt_cull_mode        cull_mode;
};


void
volt_rasterizer_create(
  volt_ctx_t                    ctx,          /* valid volt ctx */
  volt_rasterizer_t *           rasterizer,   /* out rasterizer */
  struct volt_rasterizer_desc * desc);        /* rasterizer desc */


/* ------------------------------------------------------- [ renderpass ] -- */
/*
  Renderpass
  --
  A renderpass a sequence of drawing comamnds that will render onto the target
  framebuffer.
*/


struct volt_renderpass_desc
{
  volt_framebuffer_t  fbo;                  /* optional - fbo to render to - VOLT_NULL if default backbuffer. */
  unsigned *          attachments;          /* optional - if fbo bound, and attachments VOLT_NULL then defaults to all color attachments. */
  unsigned            attachment_count;     /* required - number of attachments. */
  const char *        name;                 /* optional - if supported used in debugging. */
};


struct volt_pipeline_desc
{
  struct volt_rect2d  viewport;             /* required - sizeof viewport */
  volt_program_t      program;              /* required - program to execute */
  volt_rasterizer_t   rasterizer;           /* optional - rasterizer will use defaults if VOLT_NULL*/
  volt_input_t        input;                /* required - input format */
};


struct volt_draw_desc
{
  volt_vbo_t          vbo;                  /* required - vbo to render */
  volt_ibo_t          ibo;                  /* optional - ibo to use as index, or will draw directly */
};


void
volt_renderpass_create(
  volt_ctx_t                    ctx,        /* valid volt ctx */
  volt_renderpass_t *           pass,       /* out pass value */
  struct volt_renderpass_desc * desc);      /* renderpass desc */


void
volt_renderpass_commit(
  volt_ctx_t                    ctx,        /* valid volt ctx */
  volt_renderpass_t *           pass);      /* renderpass to submit - invalid after submission */


void
volt_renderpass_set_pipeline(
  volt_renderpass_t             pass,       /* valid renderpass */
  struct volt_pipeline_desc *   pipeline);  /* pipeline desc */


void
volt_renderpass_draw_cmd(
  volt_renderpass_t             pass,       /* valid renderpass */
  struct volt_draw_desc *       draw);      /* draw desc */


void
volt_renderpass_bind_uniform_data_cmd(
  volt_renderpass_t             pass,       /* valid renderpass */
  volt_uniform_t                uniform,    /* valid uniform */
  void *                        data);      /* ptr to the data */


void
volt_renderpass_bind_texture_buffer_cmd(
  volt_renderpass_t             pass,       /* valid renderpass */
  volt_sampler_t                sampler,    /* valid sampler */
  volt_texture_t                texture);   /* valid texture */


void
volt_renderpass_clear_cmd(
  volt_renderpass_t             pass,       /* valid renderpass */
  unsigned                      flags);     /* clear flags */


void
volt_renderpass_set_scissor_cmd(
  volt_renderpass_t             pass,       /* valid renderpass */
  struct volt_rect2d            scissor);   /* scissor area */


#ifdef __cplusplus
} /* extern */
#endif


#endif /* inc guard */
