#include <rep/rep_api.h>
#include <roa_lib/assert.h>
#include <roa_lib/fundamental.h>


void
app_test(void*arg)
{
  uint32_t obj_ids[10];
  ROA_MEM_ZERO(obj_ids);

  /* object */
  {
    struct rep_object_desc obj_desc[ROA_ARR_COUNT(obj_ids)];
    ROA_MEM_ZERO(obj_desc);

    rep_object_create(
      ROA_ARR_DATA(obj_ids),
      ROA_ARR_DATA(obj_desc),
      ROA_ARR_COUNT(obj_ids));

    int i;
    for (i = 0; i < ROA_ARR_COUNT(obj_ids); ++i)
    {
      ROA_ASSERT(obj_ids[i] != 0);
    }
  }

  /* transform */
  {
    struct rep_transform_desc trans;
    rep_transform_get(&obj_ids[0], &trans, 1);

    trans.position[0] = 1.f; trans.position[1] = 2.f; trans.position[2] = 3.f;

    rep_transform_set(&obj_ids[0], &trans, 1);
    rep_transform_get(&obj_ids[0], &trans, 1);

    ROA_ASSERT(trans.position[0] == 1.f);
    ROA_ASSERT(trans.position[1] == 2.f);
    ROA_ASSERT(trans.position[2] == 3.f);
  }

  /* camera */
  {
    uint32_t cam_id = 0;

    
  }

  /* mesh renderable */
  {

  }

  rep_app_destroy();
}


int
main()
{
  /* create instance of the engine and start */
  {
    struct rep_app_desc app_desc;
    ROA_MEM_ZERO(app_desc);
    app_desc.title = "RepAPI Demo";
    app_desc.width = 1200;
    app_desc.height = 720;
    app_desc.frame_job = app_test;

    /* start the engine */
    rep_app_create(&app_desc);
  }

  /* quiting - bye! */

  return 0;

  return 0;
}