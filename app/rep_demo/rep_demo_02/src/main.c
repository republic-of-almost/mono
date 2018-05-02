/*
  Demo App
  --
  Simple demo of API useage.
*/


#include <rep/rep_api.h>
#include <roa_lib/fundamental.h>
#include <roa_lib/rand.h>
#include <roa_math/math.h>
#include <roa_lib/assert.h>
#include <stdio.h>


/* -------------------------------------------------------------- [ Data ] -- */


uint32_t camera_obj;
uint32_t graph_obj_translate[3];


/* -------------------------------------------------------------- [ Tick ] -- */


void
camera_tick(void *arg)
{
  ROA_UNUSED(arg);

  struct rep_input_desc input_desc;
  ROA_MEM_ZERO(input_desc);
  rep_input_get(&input_desc);

  /* move camera */
  {
    uint32_t cam_id = camera_obj;

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
graph_translate_tick(void *arg)
{
  ROA_UNUSED(arg);

  struct rep_time_desc time;
  rep_time_get(&time);

  struct rep_transform_desc root_trans;
  rep_transform_get(&graph_obj_translate[0], &root_trans, 1);

  static float counter = 0.f;
  counter += time.delta_time;

  root_trans.position[0] = roa_float_cos(counter) * 4;
  root_trans.position[1] = roa_float_sin(counter) * 4;

  rep_transform_set(&graph_obj_translate[0], &root_trans, 1);
}


void
app_tick(void *arg)
{
  /* unused */
  ROA_UNUSED(arg);

  /* tick entities */
  {
    struct rep_task_desc tick_desc[2];
    ROA_MEM_ZERO(tick_desc);

    tick_desc[0].function = camera_tick;
    tick_desc[1].function = graph_translate_tick;

    rep_task_submit(ROA_ARR_DATA(tick_desc), ROA_ARR_COUNT(tick_desc));
  }

  /* quit signal */
  {
    struct rep_input_desc input_desc;
    ROA_MEM_ZERO(input_desc);
    rep_input_get(&input_desc);

    if (input_desc.kb[0].keys[REP_KB_ESC] == REP_KEYSTATE_FRAME_UP)
    {
      rep_app_destroy();
    }
  }
}


/* ------------------------------------------------------------- [ Setup ] -- */


void
app_setup_camera(void *arg)
{
  ROA_UNUSED(arg);

  /* create obj */
  {
    struct rep_object_desc obj_desc;
    ROA_MEM_ZERO(obj_desc);

    rep_object_create(
      &camera_obj,
      &obj_desc,
      1);

    ROA_ASSERT(camera_obj != 0);
  }

  /* camera */
  {
    struct rep_camera_desc cam_desc;
    ROA_MEM_ZERO(cam_desc);
    cam_desc.fov    = ROA_TAU / 10.f;
    cam_desc.width  = 800.f;
    cam_desc.height = 600.f;

    rep_camera_set(&camera_obj, &cam_desc, 1);
  }

  /* transform */
  {
    struct rep_transform_desc trans_desc;
    ROA_MEM_ZERO(trans_desc);

    trans_desc.position[0] = 0.f;
    trans_desc.position[1] = 6.f;
    trans_desc.position[2] = 20.f;

    trans_desc.scale[0] = 1.f;
    trans_desc.scale[1] = 1.f;
    trans_desc.scale[2] = 1.f;

    trans_desc.rotation[3] = 1.f;

    rep_transform_set(&camera_obj, &trans_desc, 1);
  }
}


void
app_setup_translate(void *arg)
{
  ROA_UNUSED(arg);

  printf("Create Translate\n");

  /* create obj */
  {
    struct rep_object_desc obj_desc[ROA_ARR_COUNT(graph_obj_translate)];
    ROA_MEM_ZERO(obj_desc);

    rep_object_create(
      ROA_ARR_DATA(graph_obj_translate),
      ROA_ARR_DATA(obj_desc),
      ROA_ARR_COUNT(obj_desc));

    ROA_ASSERT(graph_obj_translate[0] != 0);

    /* set parents */
    {
      int i;
      int count = ROA_ARR_COUNT(graph_obj_translate);

      for(i = 1; i < count; ++i)
      {
        obj_desc[i].object_parent = graph_obj_translate[i - 1];
      }

      rep_object_update(
        ROA_ARR_DATA(graph_obj_translate),
        ROA_ARR_DATA(obj_desc),
        ROA_ARR_COUNT(obj_desc));
    }
  }

  /* create renderables */
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

    struct rep_mesh_renderable_desc rdr_desc[ROA_ARR_COUNT(graph_obj_translate)];
    ROA_MEM_ZERO(rdr_desc);

    int i;
    int count = ROA_ARR_COUNT(graph_obj_translate);

    for(i = 0; i < count; ++i)
    {
      rdr_desc[i].material_id = material_id;
      rdr_desc[i].mesh_id = mesh_id;
    }

    rep_mesh_renderable_set(
      ROA_ARR_DATA(graph_obj_translate),
      ROA_ARR_DATA(rdr_desc),
      ROA_ARR_COUNT(rdr_desc));
  }

  /* set transforms */
  {
    struct rep_transform_desc trans_desc[ROA_ARR_COUNT(graph_obj_translate)];
    ROA_MEM_ZERO(trans_desc);

    int i;
    int count = ROA_ARR_COUNT(graph_obj_translate);

    for(i = 0; i < count; ++i)
    {
      trans_desc[i].position[0] = (float)i * 4.f;

      trans_desc[i].scale[0] = 1.f;
      trans_desc[i].scale[1] = 1.f;
      trans_desc[i].scale[2] = 1.f;

      trans_desc[i].rotation[3] = 1.f;
    }

    rep_transform_set(
      ROA_ARR_DATA(graph_obj_translate),
      ROA_ARR_DATA(trans_desc),
      ROA_ARR_COUNT(trans_desc));
  }
}


void
app_startup(void *arg)
{
  ROA_UNUSED(arg);

  /* setup entities */
  {
    struct rep_task_desc setup_desc[2];
    ROA_MEM_ZERO(setup_desc);

    setup_desc[0].function = app_setup_camera;
    setup_desc[1].function = app_setup_translate;

    rep_task_submit(ROA_ARR_DATA(setup_desc), ROA_ARR_COUNT(setup_desc));
  }

  /* change tick */
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
