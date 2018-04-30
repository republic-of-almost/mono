#include <rep/rep_api.h>
#include <data/engine_data.h>
#include <data/config.h>
#include <roa_lib/assert.h>
#include <roa_lib/alloc.h>
#include <roa_renderer/roa_renderer.h>
#include <roa_ctx/roa_ctx.h>
#include <roa_job/roa_job.h>
#include <renderer/renderer_tick.h>
#include <roa_lib/log.h>
#include <roa_lib/time.h>


rep_task application_frame_func = ROA_NULL;
void *application_frame_arg = ROA_NULL;

unsigned long last_tick = 0;



ROA_JOB(rep_game_loop_tick, rep_task)
{
  /* delta time */
  {
    unsigned long this_tick = roa_time_get_current_ms();
    unsigned long difference = this_tick - last_tick;
    last_tick = this_tick;

    rep_data_time()->delta_time = (float)difference / 1000.f;
  }

  /* new frame */
  ROA_BOOL new_frame = roa_ctx_new_frame(rep_data_ctx());
  ROA_BOOL has_tick = !!application_frame_func;

  if (new_frame == ROA_TRUE && has_tick == ROA_TRUE)
  {
    /* update input */
    {
      struct roa_ctx_keyboard_desc kb_desc;
      
      roa_ctx_keyboard_get_desc(rep_data_ctx(), &kb_desc);

      /* copy kb data */
      int i;
      int count = kb_desc.kb_key_count;

      struct rep_input_desc *rep_in_desc = rep_data_input_data();

      for (i = 0; i < count; ++i)
      {
        int roa_key = i;
        int rep_key = REP_KB_UNKNOWN;

        switch (i)
        {
          /* numeric */
          case(ROA_KB_0): rep_key = REP_KB_0; break;
          case(ROA_KB_1): rep_key = REP_KB_1; break;
          case(ROA_KB_2): rep_key = REP_KB_2; break;
          case(ROA_KB_3): rep_key = REP_KB_3; break;
          case(ROA_KB_4): rep_key = REP_KB_4; break;
          case(ROA_KB_5): rep_key = REP_KB_5; break;
          case(ROA_KB_6): rep_key = REP_KB_6; break;
          case(ROA_KB_7): rep_key = REP_KB_7; break;
          case(ROA_KB_8): rep_key = REP_KB_8; break;
          case(ROA_KB_9): rep_key = REP_KB_9; break;

          /* alpha */
          case(ROA_KB_A): rep_key = REP_KB_A; break;
          case(ROA_KB_B): rep_key = REP_KB_B; break;
          case(ROA_KB_C): rep_key = REP_KB_C; break;
          case(ROA_KB_D): rep_key = REP_KB_D; break;
          case(ROA_KB_E): rep_key = REP_KB_E; break;
          case(ROA_KB_F): rep_key = REP_KB_F; break;
          case(ROA_KB_G): rep_key = REP_KB_G; break;
          case(ROA_KB_H): rep_key = REP_KB_H; break;
          case(ROA_KB_I): rep_key = REP_KB_I; break;
          case(ROA_KB_J): rep_key = REP_KB_J; break;
          case(ROA_KB_K): rep_key = REP_KB_K; break;
          case(ROA_KB_L): rep_key = REP_KB_L; break;
          case(ROA_KB_M): rep_key = REP_KB_M; break;
          case(ROA_KB_N): rep_key = REP_KB_N; break;
          case(ROA_KB_O): rep_key = REP_KB_O; break;
          case(ROA_KB_P): rep_key = REP_KB_P; break;
          case(ROA_KB_Q): rep_key = REP_KB_Q; break;
          case(ROA_KB_R): rep_key = REP_KB_R; break;
          case(ROA_KB_S): rep_key = REP_KB_S; break;
          case(ROA_KB_T): rep_key = REP_KB_T; break;
          case(ROA_KB_U): rep_key = REP_KB_U; break;
          case(ROA_KB_V): rep_key = REP_KB_V; break;
          case(ROA_KB_W): rep_key = REP_KB_W; break;
          case(ROA_KB_X): rep_key = REP_KB_X; break;
          case(ROA_KB_Y): rep_key = REP_KB_Y; break;
          case(ROA_KB_Z): rep_key = REP_KB_Z; break;

          /* other */
          case(ROA_KB_ESC): rep_key = REP_KB_ESC; break;
          case(ROA_KB_TAB): rep_key = REP_KB_TAB; break;
        }

        int curr_roa_ctx_state = kb_desc.kb_keys[i];
        rep_keystate curr_rep_state = rep_in_desc->kb[0].keys[rep_key];

        if (curr_roa_ctx_state)
        {
          if (curr_rep_state == REP_KEYSTATE_FRAME_DOWN)
          {
            rep_in_desc->kb[0].keys[rep_key] = REP_KEYSTATE_DOWN;
          }
          else if(curr_rep_state != REP_KEYSTATE_DOWN)
          {
            rep_in_desc->kb[0].keys[rep_key] = REP_KEYSTATE_FRAME_DOWN;
          }
          else
          {
            rep_in_desc->kb[0].keys[rep_key] = REP_KEYSTATE_DOWN;
          }
        }
        else
        {
          if (curr_rep_state == REP_KEYSTATE_FRAME_UP)
          {
            rep_in_desc->kb[0].keys[rep_key] = REP_KEYSTATE_UP;
          }
          else if (curr_rep_state != REP_KEYSTATE_UP)
          {
            rep_in_desc->kb[0].keys[rep_key] = REP_KEYSTATE_FRAME_UP;
          }
          else
          {
            rep_in_desc->kb[0].keys[rep_key] = REP_KEYSTATE_UP;
          }
        }
      }

      struct roa_ctx_mouse_desc ms_desc;
      roa_ctx_mouse_get_desc(rep_data_ctx(), &ms_desc);

      rep_in_desc->ms[0].delta[0] = ms_desc.x_delta;
      rep_in_desc->ms[0].delta[1] = ms_desc.y_delta;
    }

    /* user task */
    { 
      application_frame_func(application_frame_arg);

      roa_tagged_allocator_free(rep_config_tagged_hash_logic());
    }

    /* physics tasks */
    {
      /* physics is undecided atm */
      roa_tagged_allocator_free(rep_config_tagged_hash_physics());
    }

    /* renderer tasks */
    {
      struct roa_job_desc renderer_tick;
      renderer_tick.func          = rep_renderer_tick;
      renderer_tick.arg           = ROA_NULL;
      renderer_tick.thread_locked = ROA_TRUE;

      roa_job_dispatcher_ctx_t job_ctx = rep_data_dispatcher();

      uint64_t marker = roa_job_submit(
        job_ctx,
        &renderer_tick,
        1);

      roa_job_wait(job_ctx, marker);

      roa_tagged_allocator_free(rep_config_tagged_hash_rendering());
    }

    /* submit next frame */
    {
      struct roa_job_desc tick_desc;
      tick_desc.func          = rep_game_loop_tick;
      tick_desc.arg           = ROA_NULL;
      tick_desc.thread_locked = ROA_TRUE;

      roa_job_submit(rep_data_dispatcher(), &tick_desc, 1);
    }
  }
}


void
rep_app_create(
  const struct rep_app_desc *desc)
{
  ROA_ASSERT(desc);

  if (desc)
  {
    /* init order is important */
    {
      roa_tagged_allocator_init();
      rep_config_init();
      rep_data_init();
    }

    /* set desc */
    rep_app_set(desc);

    /* tell renderer viewport */
    {
      struct roa_ctx_window_desc win_desc;
      roa_ctx_get_window_desc(rep_data_ctx(), &win_desc);

      roa_renderer_set_device_resolution(
        rep_data_renderer(),
        win_desc.width,
        win_desc.height);
    }

    /* setup job dispatcher */
    struct roa_job_desc tick_desc;
    tick_desc.arg           = application_frame_func;
    tick_desc.func          = rep_game_loop_tick;
    tick_desc.thread_locked = ROA_TRUE;

    roa_job_submit(rep_data_dispatcher(), &tick_desc, 1);

    /* start dispatcher */
    roa_job_dispatcher_ctx_run(rep_data_dispatcher());

    ROA_LOG_INFO("No Jobs in dispatcher shutting down");

    /* some data */
    last_tick = roa_time_get_current_ms();

    /* closing */
    roa_tagged_allocator_destroy();
    rep_data_destroy();
  }
}


void
rep_app_get(
  struct rep_app_desc * out_desc)
{
  /* param check */
  ROA_ASSERT(out_desc);

  if (out_desc)
  {
    struct roa_ctx_window_desc win_desc;
    roa_ctx_get_window_desc(rep_data_ctx(), &win_desc);

    out_desc->title     = win_desc.title;
    out_desc->width     = win_desc.width;
    out_desc->height    = win_desc.height;
    out_desc->frame_job = application_frame_func;
  }
}


void
rep_app_set(
  const struct rep_app_desc * desc)
{
  /* param check */
  ROA_ASSERT(desc);

  if (desc)
  {
    /* window */
    struct roa_ctx_window_desc win_desc;
    win_desc.title  = desc->title;
    win_desc.width  = desc->width;
    win_desc.height = desc->height;

    roa_ctx_set_window_desc(rep_data_ctx(), &win_desc);

    /* global application function */
    application_frame_func = desc->frame_job;
    application_frame_arg  = desc->frame_arg;
  }
}


void
rep_app_destroy()
{
  application_frame_func = ROA_NULL;
  application_frame_arg  = ROA_NULL;
}
