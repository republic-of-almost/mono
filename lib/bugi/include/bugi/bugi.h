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


/* ------------------------------------------------------- [ Environment ] -- */


struct bugi_draw_cmd
{
  int i;
};


void bugi_draw_cmnds(bugi_draw_cmds *out_data, int *out_count);


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
#define BUGI_MZERO(obj) memset(&obj, 0, sizeof(obj));


unsigned
bugi_hash(const char *text)
{
  return 0;
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
  int width;
  int height;
  int pos_x;
  int pos_y;
};


struct bugi_stage_ctx
{
  int width;
  int height;
};


struct bugi_input_ctx
{
  int ptr_state;
  int ptr_x;
  int ptr_y;
};


struct bugi_ctx
{
  bugi_window *active_windows;
  bugi_window *window_history;

  bugi_stage_ctx stage;
  bugi_input_ctx input;
} ctx;


/* ---------------------------------------------------------- [ Lifetime ] -- */


int bugi_init()
{
  bugi_destroy(); /* protects against double init's */

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

}


void bugi_end()
{

}


/* ------------------------------------------------------- [ UI Elements ] -- */


void bugi_text(const char *text)
{

}


void bugi_float3(const char *name, float *f3)
{

}


/* ------------------------------------------------------- [ Environment ] -- */


void bugi_env_viewport(int width, int height)
{

}


void bugi_env_pointer_state(int down)
{

}


void bugi_env_pointer_position(int x, int y)
{

}


#endif /* impl guard */
#endif /* impl request */