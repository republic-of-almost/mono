#ifndef VOLT_FWD_INCLUDED_
#define VOLT_FWD_INCLUDED_


#ifdef __cplusplus
extern "C" {
#endif


 /* ----------------------------------------------------------- [ types ] -- */


typedef struct volt_ctx *         volt_ctx_t;
typedef struct volt_renderpass *  volt_renderpass_t;
typedef struct volt_vbo *         volt_vbo_t;
typedef struct volt_ibo *         volt_ibo_t;
typedef struct volt_texture *     volt_texture_t;
typedef struct volt_input *       volt_input_t;
typedef struct volt_program *     volt_program_t;
typedef struct volt_rasterizer *  volt_rasterizer_t;
typedef struct volt_uniform *     volt_uniform_t;
typedef struct volt_sampler *     volt_sampler_t;
typedef struct volt_framebuffer * volt_framebuffer_t;
typedef struct volt_blend *       volt_blend_t;


#ifdef __cplusplus
} /* extern */
#endif


#endif /* inc guard */