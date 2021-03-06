#ifndef ROA_TASK_INCLUDED_56435A9E_2011_4D0D_B76D_854777D6DDDB
#define ROA_TASK_INCLUDED_56435A9E_2011_4D0D_B76D_854777D6DDDB


typedef void(*roa_renderer_task_func)(void*, void*, void*);


/* ------------------------------------------------------ [ render task ] -- */


void
task_camera_mats(void *arg1, void *arg2, void *arg3);


void
task_draw_calls(void *arg1, void *arg2, void *arg3);


void
task_decal_decay(void *arg1, void *arg2, void *arg3);


void
task_decal_update(void *arg1, void *arg2, void *arg3);


/* dummy task that is created after a draw call, to generate the first renderpasses */
void
task_submit(void *arg1, void *arg2, void *arg3);


#endif /* inc guard */
