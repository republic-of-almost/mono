#ifndef VOLT_INCLUDED_5721633F_D449_48B4_9B7D_3839CEC61DDA
#define VOLT_INCLUDED_5721633F_D449_48B4_9B7D_3839CEC61DDA


#ifdef __cplusplus
extern "C" {
#endif


/* -------------------------------------------------------- [ Volt Types ] -- */


struct volt_ctx;

typedef int VOLT_BOOL;
#define VOLT_TRUE 1
#define VOLT_FALSE 0


/* ------------------------------------------------------ [ Volt Liftime ] -- */


struct volt_ctx_desc
{
  const char **extension_list;
  unsigned extension_list_count;

  void *window_handle;
};


VOLT_BOOL
volt_ctx_create(
  struct volt_ctx         **ctx,
  struct volt_ctx_desc    *desc);


VOLT_BOOL
volt_ctx_destroy(
  struct volt_ctx         **ctx);


void
volt_ctx_execute(
  struct volt_ctx         *ctx);


/* --------------------------------------------------- [ Volt Rasterizer ] -- */


typedef enum volt_rasterizer {

  VOLT_RASTERIZER_CULLBACK,
  VOLT_RASTERIZER_CULLFRONT,
  VOLT_RASTERIZER_WINDING_ORDER_CLOCKWISE,
  VOLT_RASTERIZER_WINDING_ORDER_ANTI_CLOCKWISE,

} volt_rasterizer;


/* ------------------------------------------------------- [ Volt Vertex ] -- */


typedef enum volt_vert_attr {

  VOLT_VERTEX_FLOAT1,
  VOLT_VERTEX_FLOAT2,
  VOLT_VERTEX_FLOAT3,
  VOLT_VERTEX_FLOAT4,

} volt_vert_attr;


struct volt_vertex_desc
{
  volt_vert_attr *attr;
  unsigned count;
};


unsigned
volt_vertex_format_create(
  struct volt_ctx         *ctx,
  struct volt_vertex_desc *desc);


/* ------------------------------------------------------- [ Volt Shader ] -- */


/* ------------------------------------------------------ [ Volt Texture ] -- */


/* ------------------------------------------------ [ Volt Vertex Buffer ] -- */


struct volt_vertex_buffer_desc
{
  float *data;
  unsigned data_count;
};


unsigned
volt_vertex_buffer_create(
  struct volt_ctx                *ctx,
  struct volt_vertex_buffer_desc *desc);


/* ------------------------------------------------- [ Volt Index Buffer ] -- */


/* ------------------------------------------------- [ Volt Frame Buffer ] -- */


/* ----------------------------------------------- [ Volt Comamnd Buffer ] -- */


struct volt_cmd_buffer;


void
volt_cmd_draw(struct volt_cmd_buffer cmd);


void
volt_cmd_bind_vertex_buffer(struct volt_cmd_buffer cmd, unsigned vert_id);


void
volt_cmd_bind_index_buffer(struct void_cmd_buffer cmd, unsigned index_id);


void
volt_cmd_bind_vertex_input(struct volt_cmd_buffer cmd, unsigned vertex_id);


/* --------------------------------------------------- [ Volt Renderpass ] -- */


typedef void(*volt_renderpass_execute_fn)(struct volt_cmd_buffer, void *user_data);


struct volt_renderpass
{
  volt_renderpass_execute_fn execute;
};


void
volt_renderpass_create(
  struct volt_ctx         **ctx,
  struct volt_renderpass  pass);


#ifdef __cplusplus
} /* extern */
#endif


#endif /* inc guard */