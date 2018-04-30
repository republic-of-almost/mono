/*
  Demo App
  --
  Simple demo of API useage.
*/


#include <rep/rep_api.h>
#include <roa_lib/fundamental.h>
#include <roa_lib/rand.h>
#include <roa_math/math.h>


/* first object is camera, the rest are renderables */
uint32_t objects[512];
struct rep_transform_desc trans_desc[ROA_ARR_COUNT(objects)];


void
app_tick(void *arg)
{
  /* unused */
  ROA_UNUSED(arg);

  struct rep_input_desc input_desc;
  ROA_MEM_ZERO(input_desc);
  rep_input_get(&input_desc);

  /* quit signal */
  {
    if (input_desc.kb[0].keys[REP_KB_ESC] == REP_KEYSTATE_FRAME_UP)
    {
      rep_app_destroy();
    }
  }

  /* scale */
  {
    int count = ROA_ARR_COUNT(objects);
    int i;

    struct rep_transform_desc trans_update_desc[ROA_ARR_COUNT(objects)];
    rep_transform_get(
      ROA_ARR_DATA(objects),
      ROA_ARR_DATA(trans_update_desc),
      ROA_ARR_COUNT(trans_update_desc));

    static float time = 0.f;
    time += 0.03f;

    /* randomise the rest of the transforms */
    for (i = 1; i < count; ++i)
    {
      trans_update_desc[i].scale[0] = trans_desc[i].scale[0] + 1.f + roa_float_sin(time);
      trans_update_desc[i].scale[1] = trans_desc[i].scale[1] + 1.f + roa_float_sin(time);
      trans_update_desc[i].scale[2] = trans_desc[i].scale[2] + 1.f + roa_float_sin(time);

      trans_update_desc[i].rotation[0] = 0.f;
      trans_update_desc[i].rotation[1] = 0.f;
      trans_update_desc[i].rotation[2] = 0.f;
      trans_update_desc[i].rotation[3] = 1.f;
    }

    rep_transform_set(
      ROA_ARR_DATA(objects),
      ROA_ARR_DATA(trans_update_desc),
      ROA_ARR_COUNT(trans_update_desc));
  }

  /* move camera */
  {
    uint32_t cam_id = objects[0];

    /* movement */
    roa_float3 movement = roa_float3_zero();
    roa_quaternion rotation = roa_quaternion_default();
    {
      /* wasd */
      if (input_desc.kb[0].keys[REP_KB_W] == REP_KEYSTATE_DOWN)
      {
        movement.z -= 1;
      }
      if (input_desc.kb[0].keys[REP_KB_S] == REP_KEYSTATE_DOWN)
      {
        movement.z += 1;
      }
      if (input_desc.kb[0].keys[REP_KB_A] == REP_KEYSTATE_DOWN)
      {
        movement.x -= 1;
      }
      if (input_desc.kb[0].keys[REP_KB_D] == REP_KEYSTATE_DOWN)
      {
        movement.x += 1;
      }

      /* normalize movement */
      if (roa_float3_length(movement) > 0.f)
      {
        movement = roa_float3_normalize(movement);
      }

      /* mouse */
      static float pitch = -0.44f;
      static float yaw = 0.f;

      pitch -= (input_desc.ms[0].delta[1] * 0.001f);
      yaw -= (input_desc.ms[0].delta[0] * 0.001f);

      roa_quaternion pitch_rot = roa_quaternion_from_axis_angle(
        roa_transform_world_left(),
        pitch);

      roa_quaternion yaw_rot = roa_quaternion_from_axis_angle(
        roa_transform_world_up(),
        yaw);

      rotation = roa_quaternion_multiply(yaw_rot, pitch_rot);

      movement = roa_quaternion_rotate_vector(rotation, movement);
    }

    /* update transform */
    {
      struct rep_transform_desc transform;
      ROA_MEM_ZERO(transform);

      rep_transform_get(&cam_id, &transform, 1);

      roa_float3 scaled_movement = roa_float3_scale(movement, 0.1f);
      roa_float3 curr_pos = roa_float3_import(transform.position);
      roa_float3 new_pos = roa_float3_add(curr_pos, scaled_movement);

      roa_float3_export(new_pos, transform.position);
      roa_quaternion_export(rotation, transform.rotation);

      rep_transform_set(&cam_id, &transform, 1);
    }
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
    float start_range = -20.f;
    float end_range = +20.f;

    ROA_MEM_ZERO(trans_desc);

    /* camera transform is unique */
    trans_desc[0].position[0] = 0.f;
    trans_desc[0].position[1] = end_range * 2;
    trans_desc[0].position[2] = end_range * 2;

    trans_desc[0].scale[0] = 1.f;
    trans_desc[0].scale[1] = 1.f;
    trans_desc[0].scale[2] = 1.f;

    trans_desc[0].rotation[3] = 1.f;

    int count = ROA_ARR_COUNT(trans_desc);
    int i;

    /* randomise the rest of the transforms */
    for(i = 1; i < count; ++i)
    {
      trans_desc[i].position[0] = roa_rand_range_float(start_range, end_range);
      trans_desc[i].position[1] = roa_rand_range_float(start_range, end_range);
      trans_desc[i].position[2] = roa_rand_range_float(start_range, end_range);

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
    cam_desc.fov    = ROA_TAU / 10.f;
    cam_desc.width  = 800.f;
    cam_desc.height = 600.f;

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
