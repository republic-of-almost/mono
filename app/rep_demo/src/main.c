/*
  Demo App
  --
  Simple demo of API useage.
*/


#include <rep/rep_api.h>
#include <roa_lib/fundamental.h>
#include <roa_lib/rand.h>
#include <roa_math/math.h>


uint32_t objects[32];


void
app_tick(void *arg)
{
  ROA_UNUSED(arg);

  /* scale transforms */
  {

  }

  /* move camera */
  {

  }
}


void
app_startup(void *arg)
{
  ROA_UNUSED(arg);

  /* create objects */
  {
    struct rep_object_desc obj_desc[ROA_ARR_COUNT(objects)];
    ROA_MEM_ZERO(obj_desc);

    rep_object_create(
      ROA_ARR_DATA(objects),
      ROA_ARR_DATA(obj_desc),
      ROA_ARR_COUNT(obj_desc));
  }

  /* create renderables for each object other than the first */
  {
    uint64_t material_id = 0;
    {
      const char *name = "Foo";
      rep_material_find(&material_id, &name, 1);
    }

    uint64_t mesh_id = 0;
    {
      const char *name = "Cube";
      rep_mesh_find(&mesh_id, &name, 1);
    }

    struct rep_mesh_renderable_desc rdr_desc[ROA_ARR_COUNT(objects) - 1];
    ROA_MEM_ZERO(rdr_desc);

    int count = ROA_ARR_COUNT(rdr_desc);
    int i;

    /* set renderables */
    for(i = 0; i < count; ++i)
    {
      rdr_desc[i].material_id = material_id;
      rdr_desc[i].mesh_id     = mesh_id;
    }

    rep_mesh_renderable_set(
      &objects[1],
      ROA_ARR_DATA(rdr_desc),
      ROA_ARR_COUNT(rdr_desc));
  }

  /* set transforms - the first is at origin with scale of 1 */
  {
    struct rep_transform_desc trans_desc[ROA_ARR_COUNT(objects)];
    ROA_MEM_ZERO(trans_desc);

    /* camera transform is unique */
    trans_desc[0].scale[0] = 1.f;
    trans_desc[0].scale[1] = 1.f;
    trans_desc[0].scale[2] = 1.f;

    int count = ROA_ARR_COUNT(trans_desc);
    int i;

    /* randomise the rest of the transforms */
    for(i = 1; i < count; ++i)
    {
      trans_desc[i].position[0] = roa_rand_range_float(-10.f, +10.f);
      trans_desc[i].position[1] = roa_rand_range_float(-10.f, +10.f);
      trans_desc[i].position[2] = roa_rand_range_float(-10.f, +10.f);

      trans_desc[i].scale[0] = roa_rand_range_float(0.4f, 3.f);
      trans_desc[i].scale[1] = roa_rand_range_float(0.4f, 3.f);
      trans_desc[i].scale[2] = roa_rand_range_float(0.4f, 3.f);

      trans_desc[i].rotation[0] = 0.f;
      trans_desc[i].rotation[1] = 0.f;
      trans_desc[i].rotation[2] = 0.f;
      trans_desc[i].rotation[3] = 1.f;
    }

    rep_transform_set(
      ROA_ARR_DATA(objects),
      ROA_ARR_DATA(trans_desc),
      ROA_ARR_COUNT(trans_desc));
  }

  /* set camera - first obj is camera object */
  {
    struct rep_camera_desc cam_desc;
    ROA_MEM_ZERO(cam_desc);
    cam_desc.fov    = ROA_TAU / 8.f;
    cam_desc.width  = 1.f;
    cam_desc.height = 1.f;

    rep_camera_set(&objects[0], &cam_desc, 1);
  }

  /* update tick function */
  {
    struct rep_app_desc app_desc;
    rep_app_get(&app_desc);

    app_desc.frame_job = app_tick;
    rep_app_set(&app_desc);
  }
}


int
main()
{
  /* create instance of the engine and start */
  {
    struct rep_app_desc app_desc;
    ROA_MEM_ZERO(app_desc);
    app_desc.title      = "RepAPI Demo";
    app_desc.width      = 1200;
    app_desc.height     = 720;
    app_desc.frame_job  = app_startup;

    /* start the engine */
    rep_app_create(&app_desc);
  }

  /* quiting - bye! */

  return 0;
}
