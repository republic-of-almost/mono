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
  Nil_ctx *new_ctx = (Nil_ctx*)malloc(sizeof(Nil_ctx));
  
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
        aspect_callback_fn fn = ctx->aspect_startup_callbacks[i].callback;
        
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
      aspect_callback_fn fn = ctx->aspect_tick_callbacks[i].callback;
      
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
    nil_cpu_tasks_process(ctx->early_think_tasks, ctx->early_think_task_count);
    nil_gpu_tasks_process(ctx->pre_render_tasks, ctx->pre_render_task_count);
    
    nil_cpu_tasks_process(ctx->think_tasks, ctx->think_task_count);
    nil_gpu_tasks_process(ctx->pre_render_tasks, ctx->pre_render_task_count);
    
    nil_cpu_tasks_process(ctx->late_think_tasks, ctx->late_think_task_count);
    nil_gpu_tasks_process(ctx->pre_render_tasks, ctx->pre_render_task_count);
    
    nil_gpu_tasks_process(ctx->render_tasks, ctx->render_task_count);
    nil_gpu_tasks_process(ctx->post_render_tasks, ctx->post_render_task_count);
  }
  
  return ctx->quit_signal;
}


/* ------------------------------------------------------ [ Engine State ] -- */


float
nil_ctx_get_delta_time(Nil_ctx *ctx)
{
}


size_t
nil_ctx_graph_data_count(Nil_ctx *ctx)
{
}


const uint32_t*
nil_ctx_graph_data_ids(Nil_ctx *ctx)
{
}


const uint64_t*
nil_ctx_graph_data_details(Nil_ctx *ctx)
{
}


const math::transform*
nil_ctx_graph_data_local_transforms(Nil_ctx *ctx)
{
}


const math::transform*
nil_ctx_graph_data_world_transforms(Nil_ctx *ctx)
{
}



/* ----------------------------------------------------------- [ Aspects ] -- */


void
nil_ctx_add_aspect(Nil_ctx *ctx)
{
}


size_t
nil_ctx_aspect_count(Nil_ctx *ctx)
{
}




namespace Nil {


struct Engine::Impl
{
  std::vector<Aspect> aspects;

  Engine_settings settings;

  lib::milliseconds last_tick;
  float delta_time;
  
  bool quit = false;
};


Engine::Engine()
: m_impl(new Impl)
{
  BENCH_INIT_CPU

  LIB_ASSERT(m_impl);

  if(!m_impl)
  {
    LOG_ERROR_ONCE("Engine is in corrupted state.");
    return;
  }

  m_impl->last_tick = lib::timer::get_current_time();
  m_impl->delta_time = 0.f;
}


Engine::~Engine()
{
  LIB_ASSERT(m_impl);
  
  // -- Destroy Aspects -- //
  {
    BENCH_SCOPED_CPU(DestroyAspecrts)

    for(Aspect &asp : m_impl->aspects)
    {
      BENCH_SCOPED_CPU(AspectDestroy)

      if(asp.shutdown_fn)
      {
        asp.shutdown_fn(*this, asp);
      }

      /*
        We need to run GPU Tasks after each shutdown call as that has a thread
        context issue.
      */
      Nil::Data::Task_queues &tasks = Nil::Data::get_task_queues();
      for (auto &t : tasks.pre_render)
      {
        if (t.func)
        {
          t.func(*this, t.user_data);
        }
      }
      
      tasks.pre_render.clear();
    }
  }

  delete m_impl;
  m_impl = nullptr;
}


void
Engine::add_aspect(Nil::Aspect aspect)
{
  LIB_ASSERT(m_impl);

  if(!m_impl)
  {
    LOG_ERROR_ONCE("Engine is in corrupted state.");
    return;
  }

  m_impl->aspects.emplace_back(aspect);

  if(aspect.start_up_fn)
  {
    aspect.start_up_fn(*this, m_impl->aspects.back());
  }
}


size_t
Engine::aspect_count() const
{
  LIB_ASSERT(m_impl);

  if(!m_impl)
  {
    LOG_ERROR_ONCE("Engine is in corrupted state.");
    return false;
  }

  return m_impl->aspects.size();
}


bool
Engine::run()
{
  LIB_ASSERT(m_impl);

  BENCH_SCOPED_CPU(Tick)

  if(!m_impl)
  {
    LOG_ERROR_ONCE("Engine is in corrupted state.");
    return false;
  }

  // Delta Time
  {
    const lib::milliseconds curr_tick = lib::timer::get_current_time();
    const lib::milliseconds delta     = lib::timer::get_delta(m_impl->last_tick, curr_tick);

    m_impl->last_tick  = curr_tick;
    m_impl->delta_time = lib::timer::to_seconds(delta);
  }
  
  // Thinking
  {
    BENCH_SCOPED_CPU(Thinking)

    for(Aspect &asp : m_impl->aspects)
    {
      BENCH_SCOPED_CPU(AspectTick)

      if(asp.tick_fn)
      {
        asp.tick_fn(*this, asp);
      }
    }
    
    Graph::think(Data::get_graph_data());
    
    
    /*
      TASKS
      This is awful!
    */
    Nil::Data::Task_queues &tasks = Nil::Data::get_task_queues();
    
    for(auto &t : tasks.early_think)
    {
      if(t.func)
      {
        t.func(*this, t.user_data);
      }
    }
    
    tasks.early_think.clear();
    
    for(auto &t : tasks.pre_render)
    {
      if(t.func)
      {
        t.func(*this, t.user_data);
      }
    }
    
    tasks.pre_render.clear();
    
    // -- //
    
    for(auto &t : tasks.think)
    {
      if(t.func)
      {
        t.func(*this, t.user_data);
      }
    }
    
    tasks.think.clear();
    
    for(auto &t : tasks.pre_render)
    {
      if(t.func)
      {
        t.func(*this, t.user_data);
      }
    }
    
    tasks.pre_render.clear();
    
    // -- //
    
    for(auto &t : tasks.late_think)
    {
      if(t.func)
      {
        t.func(*this, t.user_data);
      }
    }
    
    tasks.late_think.clear();
    
    for(auto &t : tasks.pre_render)
    {
      if(t.func)
      {
        t.func(*this, t.user_data);
      }
    }
    
    tasks.pre_render.clear();
    
    // -- //
    
    for(auto &t : tasks.render)
    {
      if(t.func)
      {
        t.func(*this, t.user_data);
      }
    }
    
    tasks.render.clear();
    
    for(auto &t : tasks.post_render)
    {
      if(t.func)
      {
        t.func(*this, t.user_data);
      }
    }
    
    tasks.post_render.clear();
  }

  /*
    Check to see if any aspects are ready to quit.
  */
  {
    for(Aspect &asp : m_impl->aspects)
    {
      m_impl->quit |= asp.want_to_quit;
    }

    return !m_impl->quit;
  }
}


float
Engine::get_delta_time() const
{
  return m_impl->delta_time;
}


void
Engine::set_settings(const Engine_settings &in)
{
  m_impl->settings = in;
}


void
Engine::get_settings(Engine_settings &out)
{
  out = m_impl->settings;
}


void
Engine::get_state(Engine_state &out)
{

}


void
Engine::send_quit_signal()
{
  m_impl->quit = true;
}



// ------------------------------------------------------- [ Debugging Info ] --


size_t
Engine::graph_data_count() const
{
  return Data::get_graph_data()->node_id.size();
}


const uint32_t*
Engine::graph_data_get_ids() const
{
  return Data::get_graph_data()->node_id.data();
}


const uint64_t*
Engine::graph_data_details() const
{
  return Data::get_graph_data()->parent_depth_data.data();
}


const math::transform*
Engine::graph_data_local_transforms() const
{
  return Data::get_graph_data()->local_transform.data();
}


const math::transform*
Engine::graph_data_world_transforms() const
{
  return Data::get_graph_data()->world_transform.data();
}


} // ns
