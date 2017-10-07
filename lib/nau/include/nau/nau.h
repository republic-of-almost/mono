#ifndef NAU_INCLUDED_38EB9B7F_8D78_411D_8EF1_E08A411AC701
#define NAU_INCLUDED_38EB9B7F_8D78_411D_8EF1_E08A411AC701


/* ------------------------------------------------------------- [ Types ] -- */


struct Nau_ctx;


struct Nau_draw_cmd
{
  int clip[4];
  int count;
};


typedef void(*Nau_panel_callback)(Nau_ctx *ctx);


/* ---------------------------------------------------------- [ Lifetime ] -- */


void      nau_initialize(Nau_ctx **ctx);
void      nau_destroy(Nau_ctx **ctx);

/* -------------------------------------------------------------- [ Data ] -- */


void      nau_new_frame(Nau_ctx *ctx);
void      nau_create_draw_cmds(Nau_ctx *ctx);
void      nau_render_data(Nau_ctx *ctx, Nau_draw_cmd **renderables, int *count);


/* ---------------------------------------------------------- [ Settings ] -- */


void      nau_set_viewport(Nau_ctx *ctx, const int width, const int height);


/* ------------------------------------------------------------ [ Panels ] -- */


void      nau_register_panel(Nau_ctx *ctx, const char *name, const Nau_panel_callback cb);


/* ----------------------------------------------------------- [ Widgets ] -- */



#endif // inc guard
