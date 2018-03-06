#include <renderer/renderer.h>


ROA_JOB(renderables_startup, void*)
{
  /* load mesh program */
}


ROA_JOB(renderables_submit, struct mesh_renderable_arg*)
{
  /* convert renderables into something drawable */

}


ROA_JOB(renderables_render_pass, void*)
{
  /* generate pass data */
}


ROA_JOB(renderables_render, void*)
{
  /* submit passes */
}