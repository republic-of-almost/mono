#ifndef ROA_TASK_INCLUDED_56435A9E_2011_4D0D_B76D_854777D6DDDB
#define ROA_TASK_INCLUDED_56435A9E_2011_4D0D_B76D_854777D6DDDB


typedef void(*roa_renderer_task_func)(void*);


/* ------------------------------------------------------ [ render task ] -- */


struct task_arg
{
        struct roa_renderer_ctx *ctx;
        struct renderpass *rp;
};


void
task_camera_mats(void *arg);


void
task_draw_calls(void *arg);


void
task_decal_decay(void *arg);


void
task_decal_update(void *arg);


/* dummy task that is created after a draw call, to generate the first renderpasses */
void
task_submit(void *arg);


#endif /* inc guard */
