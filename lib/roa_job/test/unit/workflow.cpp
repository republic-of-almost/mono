#include <roa_job/dispatcher.h>
#include <catch/catch.hpp>
#include <chrono>
#include <thread>


/* ------------------------------------------------------- [ Test Helper ] -- */


#define ARR_COUNT(arr) sizeof(arr) / sizeof(arr[0])


/* --------------------------------------------------------- [ Test Func ] -- */


/* gameplay */
static int player_pos[] = {0, 1, 2};

void
job_calc_player(roa_dispatcher_ctx *, void *arg)
{
  /* do some cool calculation */
  std::this_thread::sleep_for(
    std::chrono::milliseconds(rand() % 4)
  );
  
  reinterpret_cast<int*>(arg)[0] += 1;
}

void
job_calc_enemies(roa_dispatcher_ctx *, void *)
{
  /* do some cool calculation */
  std::this_thread::sleep_for(
    std::chrono::milliseconds(rand() % 4)
  );
}

void
job_gameplay(roa_dispatcher_ctx *ctx, void *)
{
  roa_job_desc enemies_desc[]
  {
    {job_calc_enemies, player_pos},
    {job_calc_enemies, player_pos},
    {job_calc_enemies, player_pos},
    {job_calc_enemies, player_pos},
  };

  unsigned enemies_calc = roa_dispatcher_add_jobs(
    ctx,
    enemies_desc,
    ARR_COUNT(enemies_desc)
  );
  
  roa_job_desc player_update[]
  {
    {job_calc_player, player_pos},
  };
  
  unsigned job_calc_player = roa_dispatcher_add_jobs(
    ctx,
    player_update,
    ARR_COUNT(player_update)
  );
  
  roa_dispatcher_wait_for_counter(ctx, enemies_calc);
  roa_dispatcher_wait_for_counter(ctx, job_calc_player);
}

/* physics */

void
job_calc_phys_step(roa_dispatcher_ctx *, void *arg)
{
  /* do some cool calculation */
  std::this_thread::sleep_for(
    std::chrono::milliseconds(rand() % 4)
  );
  
  reinterpret_cast<int*>(arg)[1] -= 1;
}

void
job_calc_phys_collisions(roa_dispatcher_ctx *, void *)
{
  /* do some cool calculation */
  std::this_thread::sleep_for(
    std::chrono::milliseconds(rand() % 4)
  );
}


void
job_physics(roa_dispatcher_ctx *ctx, void *)
{
  roa_job_desc phys_desc[]
  {
    {job_calc_phys_collisions, nullptr},
    {job_calc_phys_collisions, nullptr},
    {job_calc_phys_collisions, nullptr},
    {job_calc_phys_collisions, nullptr},
  };

  unsigned job_calc_phys = roa_dispatcher_add_jobs(
    ctx,
    phys_desc,
    ARR_COUNT(phys_desc)
  );
  
  roa_job_desc player_phys[]
  {
    {job_calc_phys_step, player_pos},
  };
  
  
  unsigned job_calc_player = roa_dispatcher_add_jobs(
    ctx,
    player_phys,
    ARR_COUNT(player_phys)
  );
  
  roa_dispatcher_wait_for_counter(ctx, job_calc_phys);
  roa_dispatcher_wait_for_counter(ctx, job_calc_player);
}

/* renderer */

void
job_cull_renderables(roa_dispatcher_ctx *, void *)
{
  /* do some cool calculation */
  std::this_thread::sleep_for(
    std::chrono::milliseconds(rand() % 4)
  );
}

void
job_build_cmd_buffers(roa_dispatcher_ctx *, void *arg)
{
  /* do some cool calculation */
  std::this_thread::sleep_for(
    std::chrono::milliseconds(rand() % 4)
  );
  
  reinterpret_cast<int*>(arg)[2] += 1;
}

void
job_renderer(roa_dispatcher_ctx *ctx, void *)
{
  roa_job_desc cull_desc[]
  {
    {job_cull_renderables, nullptr},
    {job_cull_renderables, nullptr},
    {job_cull_renderables, nullptr},
  };


  unsigned job_cull = roa_dispatcher_add_jobs(
    ctx,
    cull_desc,
    ARR_COUNT(cull_desc)
  );
  
  roa_job_desc cmds[]
  {
    {job_build_cmd_buffers, player_pos},
  };
  
  
  unsigned job_cmds = roa_dispatcher_add_jobs(
    ctx,
    cmds,
    ARR_COUNT(cmds)
  );
  
  roa_dispatcher_wait_for_counter(ctx, job_cull);
  roa_dispatcher_wait_for_counter(ctx, job_cmds);
}

/* frame */

void
job_frame(roa_dispatcher_ctx *ctx, void *arg)
{
  int *frames = reinterpret_cast<int*>(arg);
  int frame_counter = *frames;

  /* add jobs */
  if(frame_counter)
  {
    /* gameplay */
    {
      roa_job_desc gameplay[]
      {
        {job_gameplay, nullptr},
      };
      
      
      unsigned job = roa_dispatcher_add_jobs(
        ctx,
        gameplay,
        ARR_COUNT(gameplay)
      );
      
      roa_dispatcher_wait_for_counter(ctx, job);
    }
    
    /* phys */
    {
      roa_job_desc phys[]
      {
        {job_physics, nullptr},
      };
      
      
      unsigned job = roa_dispatcher_add_jobs(
        ctx,
        phys,
        ARR_COUNT(phys)
      );
      
      roa_dispatcher_wait_for_counter(ctx, job);
    }
    
    /* render */
    {
      roa_job_desc renderer[]
      {
        {job_renderer, nullptr},
      };
      
      
      unsigned job = roa_dispatcher_add_jobs(
        ctx,
        renderer,
        ARR_COUNT(renderer)
      );
      
      roa_dispatcher_wait_for_counter(ctx, job);
    }
  }
  
  --(*frames);
}


/* --------------------------------------------------------- [ Test Case ] -- */


TEST_CASE("Optio Workflow")
{
  roa_dispatcher_ctx *ctx{nullptr};
  roa_dispatcher_create(&ctx, NULL);
  
  player_pos[0] = 0;
  player_pos[1] = 1;
  player_pos[2] = 2;
  
  auto do_frame = [&](int frames)
  {
    roa_job_desc frame[]
    {
      {job_frame, &frames},
    };
    
    roa_dispatcher_add_jobs(
      ctx,
      frame,
      ARR_COUNT(frame)
    );
    
    roa_dispatcher_run(ctx);
    
    REQUIRE(player_pos[0] == 1);
    REQUIRE(player_pos[1] == 0);
    REQUIRE(player_pos[2] == 3);
  };

  SECTION("Single Frame")
  {
    int frames = 1;
    do_frame(frames);
  }
  
  SECTION("10 Frames")
  {
    int frames = 10;
    do_frame(frames);
  }
  
  roa_dispatcher_destroy(&ctx);
}