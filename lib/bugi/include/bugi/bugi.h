#ifndef BUGI_INCLUDED_
#define BUGI_INCLUDED_


/* ---------------------------------------------------------- [ Lifetime ] -- */


int bugi_init();
void bugi_new_frame();
int bugi_destroy();


/* ------------------------------------------------------------ [ Window ] -- */


void bugi_begin(const char *win_name);
void bugi_end();


/* ------------------------------------------------------- [ UI Elements ] -- */


void bugi_text(const char *text);
void bugi_float(const char *name, float *f);
void bugi_float2(const char *name, float *f2);
void bugi_float3(const char *name, float *f3);
void bugi_float4(const char *name, float *f4);
int bugi_checkbox(const char *name, int *checked);
int bugi_button(const char *name);


/* --------------------------------------------------- [ Render Commands ] -- */


struct bugi_draw_cmd
{
  int i;
};


void bugi_draw_cmds(struct bugi_draw_cmd *out_data, int *out_count);


/* ------------------------------------------------------- [ Environment ] -- */


void bugi_env_viewport(int width, int height);
void bugi_env_pointer_state(int down);
void bugi_env_pointer_position(int x, int y);


#endif /* inc guard */


/******************************************************************************/
/******************************* IMPLEMENTATION *******************************/
/******************************************************************************/


#ifdef BUGI_IMPL
#ifndef BUGI_IMPL_INCLUDED_
#define BUGI_IMPL_INCLUDED_


/* ---------------------------------------------------- [ Base Functions ] -- */


#include <stdlib.h>
#define BUGI_MZERO(obj) memset(&obj, 0, sizeof(obj))


#include <assert.h>
#define BUGI_ASSERT(expr) assert(expr)


unsigned
bugi_hash(const char *text)
{
  unsigned hash = 5381;

  int c;
  while (c = *text++)
  {
    hash = ((hash << 5) + hash) + c;
  }

  return hash;
}


/* ------------------------------------------------------- [ Bugi Buffer ] -- */


struct bugi_buffer
{
  void *data;

  int size;
};


/* ------------------------------------------------------- [ State Types ] -- */


struct bugi_window
{
  unsigned window_id;
  int width, height, pos_x, pos_y;
};


struct bugi_stage_ctx
{
  int width, height;
};


typedef enum bugi_ptr_state {
  bugi_ptr_up,
  bugi_ptr_up_this_frame,
  bugi_ptr_down,
  bugi_ptr_down_this_frame,
} bugi_ptr_state;


struct bugi_input_ctx
{
  bugi_ptr_state ptr_state;
  int pos_x, pos_y;
};


struct bugi_ctx
{
  struct bugi_window *active_windows;
  struct bugi_window *window_history;

  struct bugi_stage_ctx stage;
  struct bugi_input_ctx input;
} ctx;


/* ---------------------------------------------------------- [ Lifetime ] -- */


int bugi_init()
{
  BUGI_MZERO(ctx);

  return 1;
}


void bugi_new_frame()
{

}


int bugi_destroy()
{

}


/* ------------------------------------------------------------ [ Window ] -- */


void bugi_begin(const char *win_name)
{
  BUGI_ASSERT(win_name); /* name is required */
  BUGI_ASSERT(0); /* has active window, did you call bugi_end() */

  unsigned win_id = bugi_hash(win_name);

  /* search history */

  /* add window to active windows */
}


void bugi_end()
{
  BUGI_ASSERT(0); /* no active window, did you call bugi_begin() */

  /* close off current active window */
}


/* ------------------------------------------------------- [ UI Elements ] -- */


void bugi_text(const char *text)
{

}


void bugi_float(const char *name, float *f)
{

}


void bugi_float2(const char *name, float *f2)
{

}


void bugi_float3(const char *name, float *f3)
{

}


void bugi_float4(const char *name, float *f4)
{

}


int bugi_checkbox(const char *name, int *checked)
{
  return 0;
}


int bugi_button(const char *name)
{
  return 0;
}


/* --------------------------------------------------- [ Render Commands ] -- */


void bugi_draw_cmds(struct bugi_draw_cmd *out_data, int *out_count)
{

}


/* ------------------------------------------------------- [ Environment ] -- */


void bugi_env_viewport(int width, int height)
{
  ctx.stage.width = width;
  ctx.stage.height = height;
}


void bugi_env_pointer_state(int down)
{
  ctx.input.ptr_state = down ? bugi_ptr_down_this_frame : bugi_ptr_up_this_frame;
}


void bugi_env_pointer_position(int x, int y)
{
  ctx.input.pos_x = x;
  ctx.input.pos_y = y;
}


#endif /* impl guard */
#endif /* impl request */