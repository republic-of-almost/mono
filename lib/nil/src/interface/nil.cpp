#include <nil/nil.hpp>
#include <nil/node.hpp>
#include <nil/resource/texture.hpp>
#include <nil/aspect.hpp>
#include <nil/task.hpp>
#include <data/internal_data.hpp>
#include <data/task_queue.hpp>
#include <graph/graph.hpp>
#include <graph/graph_data.hpp>
#include <lib/array.hpp>
#include <lib/assert.hpp>
#include <lib/bench.hpp>
#include <lib/logging.hpp>
#include <lib/assert.hpp>
#include <lib/timer.hpp>
#include <stdlib.h>


/* ---------------------------------------------------------- [ Lifetime ] -- */


int
nil_ctx_initialize(Nil_ctx **ctx)
{
  const size_t bytes = sizeof(Nil_ctx);
  Nil_ctx *new_ctx = (Nil_ctx*)malloc(bytes);
  memset(new_ctx, 0, bytes);
  
  if(new_ctx)
  {
    *ctx = new_ctx;
  }
  
  return 0;
}


void
nil_ctx_destroy(Nil_ctx **ctx)
{
  free(*ctx);
}


void
nil_ctx_quit_signal(Nil_ctx *ctx)
{
  ctx->quit_signal = true;
}


bool
nil_ctx_think(Nil_ctx *ctx)
{
  BENCH_SCOPED_CPU(Tick)

  // Delta Time
  {
    const lib::milliseconds curr_tick = lib::timer::get_current_time();
    const lib::milliseconds delta     = lib::timer::get_delta(ctx->last_tick, curr_tick);

    ctx->last_tick  = curr_tick;
    ctx->delta_time = lib::timer::to_seconds(delta);
  }
  
  /* aspects */
  {
    /* startup */
    if(ctx->aspect_startup_callback_count)
    {
      const size_t start_count = ctx->aspect_startup_callback_count;
      
      for(size_t i = 0; i < start_count; ++i)
      {
        Nil_aspect_callback_fn fn = ctx->aspect_startup_callbacks[i].callback;
        
        if(fn)
        {
          void *data = ctx->aspect_startup_callbacks[i].user_data;
          fn(ctx, data);
        }
      }
      
      ctx->aspect_startup_callback_count = 0;
    }
    
    /* tick */
    const size_t tick_count = ctx->aspect_tick_callback_count;
    
    for(size_t i = 0 ; i < tick_count; ++i)
    {
      Nil_aspect_callback_fn fn = ctx->aspect_tick_callbacks[i].callback;
      
      for(size_t i = 0; i < tick_count; ++i)
      {
        if(fn)
        {
          void *data= ctx->aspect_tick_callbacks[i].user_data;
          fn(ctx, data);
        }
      }
    }
  }
  
  Nil::Graph::think(Nil::Data::get_graph_data());
  
  /* tasks */
  {
    nil_task_cpu_process(ctx, ctx->early_think_tasks, ctx->early_think_task_count);
    nil_task_gpu_process(ctx, ctx->pre_render_tasks,  ctx->pre_render_task_count);
    
    nil_task_cpu_process(ctx, ctx->think_tasks,       ctx->think_task_count);
    nil_task_gpu_process(ctx, ctx->pre_render_tasks,  ctx->pre_render_task_count);
    
    nil_task_cpu_process(ctx, ctx->late_think_tasks,  ctx->late_think_task_count);
    nil_task_gpu_process(ctx, ctx->pre_render_tasks,  ctx->pre_render_task_count);
    
    nil_task_gpu_process(ctx, ctx->render_tasks,      ctx->render_task_count);
    nil_task_gpu_process(ctx, ctx->post_render_tasks, ctx->post_render_task_count);
  }
  
  return ctx->quit_signal;
}


/* ------------------------------------------------------ [ Engine State ] -- */


float
nil_ctx_get_delta_time(Nil_ctx *ctx)
{
  return ctx->delta_time;
}


size_t
nil_ctx_graph_data_count(Nil_ctx *ctx)
{
  return Nil::Data::get_graph_data()->node_id.size();
}


const uint32_t*
nil_ctx_graph_data_ids(Nil_ctx *ctx)
{
  return Nil::Data::get_graph_data()->node_id.data();
}


const uint64_t*
nil_ctx_graph_data_details(Nil_ctx *ctx)
{
  return Nil::Data::get_graph_data()->parent_depth_data.data();
}


const math::transform*
nil_ctx_graph_data_local_transforms(Nil_ctx *ctx)
{
  return Nil::Data::get_graph_data()->local_transform.data();
}


const math::transform*
nil_ctx_graph_data_world_transforms(Nil_ctx *ctx)
{
  return Nil::Data::get_graph_data()->world_transform.data();
}


/* ----------------------------------------------------------- [ Aspects ] -- */


void
nil_ctx_add_aspect(Nil_ctx *ctx, Nil_aspect aspect)
{
  if(ctx->current_aspect_count >= NIL_MAX_ASPECT_COUNT)
  {
    LIB_ASSERT(false);
    LOG_ERROR("Maxed out of aspect slots build with more slots");
    return;
  }

  if(aspect.startup)
  {
    const size_t index = ctx->aspect_startup_callback_count++;
    ctx->aspect_startup_callbacks[index].callback = aspect.startup;
    ctx->aspect_startup_callbacks[index].user_data = aspect.data;
    
    ctx->aspect_startup_callback_count += 1;
  }
  
  if(aspect.tick)
  {
    const size_t index = ctx->aspect_tick_callback_count++;
    ctx->aspect_tick_callbacks[index].callback = aspect.tick;
    ctx->aspect_tick_callbacks[index].user_data = aspect.data;
    
    ctx->aspect_tick_callback_count += 1;
  }
  
  if(aspect.shutdown)
  {
    const size_t index = ctx->aspect_shutdown_callback_count++;
    ctx->aspect_shutdown_callbacks[index].callback = aspect.shutdown;
    ctx->aspect_shutdown_callbacks[index].user_data = aspect.data;
    
    ctx->aspect_shutdown_callback_count += 1;
  }
  
  if(aspect.ui_menu)
  {
    const size_t index = ctx->aspect_ui_menu_callback_count++;
    ctx->aspect_ui_menu_callbacks[index].callback = aspect.ui_menu;
    ctx->aspect_ui_menu_callbacks[index].user_data = aspect.data;
    
    ctx->aspect_ui_menu_callback_count += 1;
  }
  
  if(aspect.ui_window)
  {
    const size_t index = ctx->aspect_ui_window_callback_count++;
    ctx->aspect_ui_window_callbacks[index].callback = aspect.ui_window;
    ctx->aspect_ui_window_callbacks[index].user_data = aspect.data;
    
    ctx->aspect_ui_window_callback_count += 1;
  }
  
  ctx->current_aspect_count += 1;
}


size_t
nil_ctx_aspect_count(Nil_ctx *ctx)
{
  return ctx->current_aspect_count;
}
