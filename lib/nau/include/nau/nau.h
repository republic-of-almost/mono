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


typedef void(*Nau_panel_callback)(Nau_ctx *ctx);


/* ---------------------------------------------------------- [ Lifetime ] -- */


void      nau_initialize(Nau_ctx **ctx);
void      nau_destroy(Nau_ctx **ctx);
void      nau_new_frame(Nau_ctx *ctx);


/* -------------------------------------------------------------- [ Data ] -- */


void      nau_get_vbo(Nau_ctx *ctx, float **vbo, unsigned int *count);
void      nau_get_idx(Nau_ctx *ctx, unsigned int **idx, unsigned int *count);
void      nau_get_cmds(Nau_ctx *ctx, Nau_draw_cmd **cmds, unsigned int *count);
void      nau_get_viewport(Nau_ctx *ctx, int *width, int *height);

/* ---------------------------------------------------------- [ Settings ] -- */


void      nau_set_viewport(Nau_ctx *ctx, const int width, const int height);


/* ------------------------------------------------------------ [ Window ] -- */


void      nau_begin(Nau_ctx *ctx, const char *name);
void      nau_end(Nau_ctx *ctx);


/* ----------------------------------------------------------- [ Widgets ] -- */



#endif // inc guard
