#include <rep/rep_api.h>
#include <roa_lib/fundamental.h>


enum {
  OBJ_CAMERA = 0,
  OBJ_RENDERABLES_START = 1,

  OBJ_COUNT = 128,
};


uint32_t objects[OBJ_COUNT];


void
rep_job_func(void *arg)
{
  ROA_UNUSED(arg);
}


void
rep_frame_func(void *arg)
{
	ROA_UNUSED(arg);

  struct rep_task_desc desc[1];

  desc[0].arg = 0;
  desc[0].function = rep_job_func;

  rep_task_submit(&desc[0], 1);
}


void
rep_func_setup(void *arg)
{
	ROA_UNUSED(arg);

  /* setup application data */
  {
		/* objects */
    struct rep_object_desc obj_desc[ROA_ARR_COUNT(objects)];
    ROA_MEM_ZERO(obj_desc);

    rep_object_create(
			ROA_ARR_DATA(objects),
			ROA_ARR_DATA(obj_desc),
			ROA_ARR_COUNT(obj_desc));

		/* camera */
		struct rep_camera_desc cam_desc;
		ROA_MEM_ZERO(cam_desc);

		rep_camera_set(&objects[OBJ_CAMERA], &cam_desc, 1);

		/* renderables */
		struct rep_mesh_renderable_desc rdr_desc[OBJ_COUNT - OBJ_RENDERABLES_START];
		ROA_MEM_ZERO(rdr_desc);

		rep_mesh_renderable_set(
			&objects[OBJ_RENDERABLES_START],
			ROA_ARR_DATA(rdr_desc),
			ROA_ARR_COUNT(rdr_desc));
	}

  /* reset frame job */
  {
    struct rep_app_desc app_desc;
    rep_app_get(&app_desc);
    app_desc.frame_job = rep_frame_func;
    app_desc.title = "ROA Func Test";
		rep_app_set(&app_desc);
  }
}


int
main()
{
  /* create rep application */
  {
    struct rep_app_desc app_desc;
    app_desc.title     = "Loading ROA Func Test ...";
    app_desc.width     = 1200;
    app_desc.height    = 720;
    app_desc.frame_job = rep_func_setup;

    rep_app_create(&app_desc);
  }

  return 0;
}
