#ifndef ROA_TASK_INCLUDED_56435A9E_2011_4D0D_B76D_854777D6DDDB
#define ROA_TASK_INCLUDED_56435A9E_2011_4D0D_B76D_854777D6DDDB


typedef void(*roa_renderer_task_func)(void*);


/* ------------------------------------------------------ [ render task ] -- */


struct task_render_arg
{
  int i;
};


void
task_renderpass(void *arg);


#endif /* inc guard */
