#ifndef VOLT_INCLUDED_F1E11FF6_C036_4211_A608_BA0DC5DA0259
#define VOLT_INCLUDED_F1E11FF6_C036_4211_A608_BA0DC5DA0259


#ifdef __cplusplus
extern "C" {
#endif


 /* ----------------------------------------------------------- [ types ] -- */


typedef struct volt_ctx * volt_ctx_t;
typedef struct volt_renderpass * volt_renderpass_t;
typedef struct volt_vbo * volt_vbo_t;
typedef struct volt_ibo * volt_ibo_t;
typedef struct volt_texture * volt_texture_t;
typedef struct volt_input * volt_input_t;
typedef struct volt_program * volt_program_t;
typedef struct volt_rasterizer * volt_rasterizer_t;


#define VOLT_NULL 0


/* --------------------------------------------------------- [ lifetime ] -- */


void
volt_ctx_create(volt_ctx_t *ctx);


void
volt_ctx_destroy(volt_ctx_t *ctx);


void
volt_ctx_execute(volt_ctx_t ctx);


/* -------------------------------------------------------- [ resources ] -- */


typedef enum _volt_resource_status {
  VOLT_RSRC_PENDING_CREATE,
  VOLT_RSRC_FAILED,
  VOLT_RSRC_VALID,
  VOLT_RSRC_PENDING_DESTROY,
} volt_resource_status;


/* ------------------------------------------------------- [ rsrc input ] -- */


typedef enum _volt_input_attribute {
  VOLT_INPUT_FLOAT4,
  VOLT_INPUT_FLOAT3,
  VOLT_INPUT_FLOAT2,
  VOLT_INPUT_FLOAT,

} volt_input_attribute;


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


typedef enum _volt_shader_stage {
  VOLT_SHD_VERTEX,
  VOLT_SHD_GEOMETRY,
  VOLT_SHD_FRAGMENT,
  VOLT_SHD_STAGE_COUNT,
} volt_shader_stage;


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
  volt_renderpass_t *pass);


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
volt_renderpass_bind_program(
  volt_renderpass_t pass,
  volt_program_t program);


void
volt_renderpass_draw(
  volt_renderpass_t pass);


#ifdef __cplusplus
} /* extern */
#endif


#endif /* inc guard */