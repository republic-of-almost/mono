#include <optio/dispatcher.h>
#include <stdint.h>


uint64_t ground;
uint64_t free_camera;


#define REPO_JOB(job_name, arg) void job_name(arg)


REPO_JOB(setup_scene, int *foo)
{
  job_desc[1];
  job_desc[1].func = my_func;
  job_desc[1].arg = my_arg;
  
  job_span;
  job_span.data = job_desc;
  job_span.count = 1;

  unsigned batch = repo_job_submit(list_o_jobs, 123);
  
  repo_job_wait(batch);
}


uint64_t
repo_object_create()
{

}


struct material_desc
{
};

struct mesh_desc
{
};


int
repo_object_data_add_renderable(uint64_t obj, material_desc, mesh_desc)
{
}


void
application_setup()
{
  ground = repo_object_create();
  
  repo_object_data_add_renderable(ground, material_desc{}, mesh_desc{});
  
  
  free_camera = repo_object_create();
};


int
main()
{
  optio_dispatcher_ctx *dispatcher_ctx;
  optio_dispatcher_create(&dispatcher_ctx);
  
  
  
  optio_dispatcher_destroy(&dispatcher_ctx);
  

  return 0;
}
