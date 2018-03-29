#ifndef ROA_TASK_INCLUDED_56435A9E_2011_4D0D_B76D_854777D6DDDB
#define ROA_TASK_INCLUDED_56435A9E_2011_4D0D_B76D_854777D6DDDB


typedef void(*roa_renderer_task_func)(void*);


/* ----------------------------------------------------- [ culling task ] -- */


struct task_sphere_culling_arg
{
  int i;
};


void
task_sphere_culling(void *arg);


/* ------------------------------------------------------ [ render task ] -- */


struct task_render_arg
{
  int i;
};


void
task_render(void *arg);


#endif /* inc guard */
