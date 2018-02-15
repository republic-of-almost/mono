#ifndef ROA_CTX_INCLUDED_FA8E98F2_C821_45D7_8EA6_323EA2ACD84B
#define ROA_CTX_INCLUDED_FA8E98F2_C821_45D7_8EA6_323EA2ACD84B


#include <roa_lib/fundamental.h>


#ifdef __cplusplus
extern "C" {
#endif


typedef struct roa_ctx * roa_ctx_t;


/* --------------------------------------------------------- [ lifetime ] -- */


void
roa_ctx_create(roa_ctx_t *ctx);


void
roa_ctx_destroy(roa_ctx_t *ctx);


ROA_BOOL
roa_ctx_new_frame(roa_ctx_t ctx);


/* ----------------------------------------------------------- [ window ] -- */


struct roa_ctx_window_desc
{
  int width;
  int height;
  ROA_BOOL fullscreen;
  const char *title;
};

void
roa_ctx_get_window_desc(
  roa_ctx_t ctx,
  struct roa_ctx_window_desc *out_desc);


void
roa_ctx_set_window_desc(
  roa_ctx_t ctx,
  const struct roa_ctx_window_desc *desc);


#ifdef __cplusplus
} /* extern */
#endif


#endif /* inc guard */