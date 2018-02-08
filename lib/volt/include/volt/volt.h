#ifndef VOLT_INCLUDED_F1E11FF6_C036_4211_A608_BA0DC5DA0259
#define VOLT_INCLUDED_F1E11FF6_C036_4211_A608_BA0DC5DA0259


#ifdef __cplusplus
extern "C" {
#endif


typedef struct volt_ctx * volt_ctx_t;
typedef struct volt_renderpass * volt_renderpass_t;
typedef struct volt_vbo * volt_vbo_t;
typedef struct volt_ibo * volt_ibo_t;
typedef struct volt_texture * volt_texture_t;
typedef struct volt_input * volt_input_t;
typedef struct volt_program * volt_program_t;


#define VOLT_NULL 0


/* lifetime */


void
volt_ctx_create(volt_ctx_t *ctx);


void
volt_ctx_destroy(volt_ctx_t *ctx);


void
volt_ctx_execute(volt_ctx_t ctx);


/* resources */


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


/* renderpass */


void
volt_renderpass_create(
  volt_ctx_t ctx,
  volt_renderpass_t *pass);


void
volt_renderpass_commit(
  volt_ctx_t ctx,
  volt_renderpass_t *pass);


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