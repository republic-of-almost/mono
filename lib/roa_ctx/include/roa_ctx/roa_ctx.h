#ifndef ROA_CTX_INCLUDED_FA8E98F2_C821_45D7_8EA6_323EA2ACD84B
#define ROA_CTX_INCLUDED_FA8E98F2_C821_45D7_8EA6_323EA2ACD84B


#include <roa_lib/fundamental.h>


#ifdef __cplusplus
extern "C" {
#endif


typedef struct roa_ctx * roa_ctx_t;


/* ---------------------------------------------------------- [ lifetime ] -- */


void
roa_ctx_create(roa_ctx_t *ctx);


void
roa_ctx_destroy(roa_ctx_t *ctx);


ROA_BOOL
roa_ctx_new_frame(roa_ctx_t ctx);


/* ------------------------------------------------------------ [ window ] -- */


struct roa_ctx_window_desc
{
  int width;
  int height;
  const char *title;
  int capture_mouse;
};

void
roa_ctx_get_window_desc(
  roa_ctx_t ctx,
  struct roa_ctx_window_desc *out_desc);


void
roa_ctx_set_window_desc(
  roa_ctx_t ctx,
  const struct roa_ctx_window_desc *desc);


/* ---------------------------------------------------------- [ ms input ] -- */


typedef enum ms_button_id {
  ROA_MS_BUTTON_LEFT, ROA_MS_BUTTON_MIDDLE, ROA_MS_BUTTON_RIGHT,
  ROA_MS_BUTTON_0, ROA_MS_BUTTON_1, ROA_MS_BUTTON_2, ROA_MS_BUTTON_3,
  ROA_MS_BUTTON_4,

  ROA_MS_BUTTON_COUNT
} ms_button_id;


struct roa_ctx_mouse_desc
{
  int *ms_keys;
  int ms_key_count;

  float x_pos;
  float y_pos;

  float x_delta;
  float y_delta;

  float x_scroll;
  float y_scroll;
};


void
roa_ctx_mouse_get_desc(
	roa_ctx_t ctx,
	struct roa_ctx_mouse_desc *out_desc);


/* ---------------------------------------------------------- [ kb input ] -- */


typedef enum kb_button_id
{
  ROA_KB_UNKNOWN,

  ROA_KB_0, ROA_KB_1, ROA_KB_2, ROA_KB_3, ROA_KB_4, ROA_KB_5, ROA_KB_6,
  ROA_KB_7, ROA_KB_8, ROA_KB_9,

  ROA_KB_A, ROA_KB_B, ROA_KB_C, ROA_KB_D, ROA_KB_E, ROA_KB_F, ROA_KB_G,
  ROA_KB_H, ROA_KB_I, ROA_KB_J, ROA_KB_K, ROA_KB_L, ROA_KB_M, ROA_KB_N,
  ROA_KB_O, ROA_KB_P, ROA_KB_Q, ROA_KB_R, ROA_KB_S, ROA_KB_T, ROA_KB_U,
  ROA_KB_V, ROA_KB_W, ROA_KB_X, ROA_KB_Y, ROA_KB_Z,

  ROA_KB_UP, ROA_KB_DOWN, ROA_KB_LEFT, ROA_KB_RIGHT,

  ROA_KB_F1, ROA_KB_F2, ROA_KB_F3, ROA_KB_F4, ROA_KB_F5, ROA_KB_F6,
  ROA_KB_F7, ROA_KB_F8, ROA_KB_F9, ROA_KB_F10, ROA_KB_F11, ROA_KB_F12,
  ROA_KB_F13, ROA_KB_F14, ROA_KB_F15,

  ROA_KB_ESC, ROA_KB_TAB,

  ROA_KB_COUNT
} kb_button_id;


struct roa_ctx_keyboard_desc
{
  int *kb_keys;
  int kb_key_count;
};


void
roa_ctx_keyboard_get_desc(
  roa_ctx_t ctx,
  struct roa_ctx_keyboard_desc *out_desc);


#ifdef __cplusplus
} /* extern */
#endif


#endif /* inc guard */
