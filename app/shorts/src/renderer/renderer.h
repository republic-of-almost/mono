#ifndef RENDERER_INCLUDED_B56AE667_C633_42DB_94F9_55BDD820B6CF
#define RENDERER_INCLUDED_B56AE667_C633_42DB_94F9_55BDD820B6CF


#include <volt/fwd.h>
#include <roa_job/dispatcher.h>


struct renderer_data
{
  int i;
};


struct mesh_renderable
{
  volt_vbo_t vbo;
};


struct mesh_renderable_arg
{
  struct mesh_renderable *renderables;
  float *renderable_world_mats;

  unsigned renderable_count;
};


ROA_JOB_DECL(renderables_startup);
ROA_JOB_DECL(renderables_submit);
ROA_JOB_DECL(renderables_render);


#endif /* inc guard */