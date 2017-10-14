#ifndef NAU_INCLUDED_38EB9B7F_8D78_411D_8EF1_E08A411AC701
#define NAU_INCLUDED_38EB9B7F_8D78_411D_8EF1_E08A411AC701


/* ------------------------------------------------------------- [ Types ] -- */


struct Nau_ctx;


struct Nau_draw_cmd
{
  int clip[4];
  unsigned int offset;
  unsigned int count;
};


typedef enum {
  
  NAU_PTR_STATUS_UP = 0,
  NAU_PTR_STATUS_DOWN = 1,
  
} Nau_ptr_status;


/* ---------------------------------------------------------- [ Lifetime ] -- */


void      nau_initialize(Nau_ctx **ctx);
void      nau_destroy(Nau_ctx **ctx);
void      nau_new_frame(Nau_ctx *ctx);


/* -------------------------------------------------------------- [ Data ] -- */


void      nau_get_vbo(Nau_ctx *ctx, float **vbo, int *count);
void      nau_get_idx(Nau_ctx *ctx, unsigned int **idx, int *count);
void      nau_get_cmds(Nau_ctx *ctx, Nau_draw_cmd **cmds, int *count);
void      nau_get_viewport(Nau_ctx *ctx, int *width, int *height);


/* ---------------------------------------------------------- [ Settings ] -- */


void      nau_set_viewport(Nau_ctx *ctx, int width, int height);
void      nau_set_pointer_coords(Nau_ctx *ctx, int x, int y);
void      nau_set_pointer_status(Nau_ctx *ctx, Nau_ptr_status status);


/* ------------------------------------------------------------ [ Window ] -- */


void      nau_begin(Nau_ctx *ctx, const char *name);
void      nau_end(Nau_ctx *ctx);


/* ----------------------------------------------------------- [ Widgets ] -- */


bool      nau_button(Nau_ctx *ctx, const char *name);


/* -------------------------------------------------------- [ Formatting ] -- */


void      nau_inline(Nau_ctx *ctx);
void      nau_line_break(Nau_ctx *ctx);


#endif // inc guard
