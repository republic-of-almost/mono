#ifndef NAU_INCLUDED_38EB9B7F_8D78_411D_8EF1_E08A411AC701
#define NAU_INCLUDED_38EB9B7F_8D78_411D_8EF1_E08A411AC701



/* ---------------------------------------------------------- [ Lifetime ] -- */
/*
  NAU holds all its data in a context. You need to create a context and destroy
  it. You need to call new_frame each tick of your application.
*/

struct Nau_ctx;


void      nau_initialize(Nau_ctx **ctx);
void      nau_destroy(Nau_ctx **ctx);
void      nau_new_frame(Nau_ctx *ctx);


/* -------------------------------------------------------------- [ Data ] -- */
/*
  To interact with NAU you will need to get the data from it. The primary way
  todo this is to stream the VBO and IBO into your buffers, and use the draw 
  commands to index this data.
*/

struct Nau_draw_cmd
{
  int clip[4];
  unsigned int offset;
  unsigned int count;
};


void      nau_get_vbo(Nau_ctx *ctx, float **vbo, int *count);
void      nau_get_idx(Nau_ctx *ctx, unsigned int **idx, int *count);
void      nau_get_cmds(Nau_ctx *ctx, Nau_draw_cmd **cmds, int *count);
void      nau_get_viewport(Nau_ctx *ctx, int *width, int *height);


/* ---------------------------------------------------------- [ Settings ] -- */
/*
  Nau has no knowlage of its environment so it needs to be told about changes.
*/

typedef enum {
  
  NAU_PTR_STATUS_UP   = 0,
  NAU_PTR_STATUS_DOWN = 1,
  
} Nau_ptr_status;


void      nau_set_viewport(Nau_ctx *ctx, int width, int height);
void      nau_set_pointer_coords(Nau_ctx *ctx, int x, int y);
void      nau_set_pointer_status(Nau_ctx *ctx, Nau_ptr_status status);


/* ------------------------------------------------------------ [ Window ] -- */
/*
  Every window must start with a begin and end call, all widgets and formatting
  calls must happen in between these begin and end calls.
  
  You can customise a window by calling the various property setters. These 
  properties only affect the next window created and will be reset when end is
  called.
*/

typedef enum {

  NAU_WIN_NO_RESIZE = 1 << 0,
  NAU_WIN_NO_DRAG   = 1 << 1,
  NAU_WIN_NO_CLOSE  = 1 << 2,
  NAU_WIN_NO_MIN    = 1 << 3,

} Nau_win_flags;


void      nau_win_prop_flags(Nau_ctx *ctx, int flags);
void      nau_win_prop_size(Nau_ctx *ctx, float size[2]);
void      nau_win_prop_position(Nau_ctx *ctx, float position[2]);

void      nau_begin(Nau_ctx *ctx, const char *name);
void      nau_end(Nau_ctx *ctx);


/* ----------------------------------------------------------- [ Widgets ] -- */
/*
  Inside begin and end you can call widgit functions to add content and
  functionality to the window.
*/


void      nau_text(Nau_ctx *ctx, const char *text);
bool      nau_button(Nau_ctx *ctx, const char *name);


/* -------------------------------------------------------- [ Formatting ] -- */
/*
  These are some extra formatting options that you may call between begin and 
  end functions.
*/


void      nau_line_break(Nau_ctx *ctx);


#endif // inc guard
