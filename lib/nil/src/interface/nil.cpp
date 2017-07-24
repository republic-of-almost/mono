#include <nil/nil.hpp>
#include <nil/node.hpp>
#include <nil/aspect.hpp>
#include <data/internal_data.hpp>
#include <graph/graph.hpp>
#include <graph/graph_data.hpp>
#include <lib/array.hpp>
#include <lib/assert.hpp>
#include <lib/bench.hpp>
#include <lib/logging.hpp>
#include <lib/assert.hpp>
#include <lib/timer.hpp>


namespace Nil {


struct Engine::Impl
{
  std::vector<Aspect> aspects;

  Engine_settings settings;

  lib::milliseconds last_tick;
  float delta_time;
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
    const lib::milliseconds delta = lib::timer::get_delta(m_impl->last_tick, curr_tick);

    printf("mil: %d\n", delta);

    m_impl->last_tick = curr_tick;

    m_impl->delta_time = lib::timer::to_seconds(delta);
  }
  // Distro Events
  {
    BENCH_SCOPED_CPU(DistroEvents)

    for(Aspect &asp : m_impl->aspects)
    {
      if(asp.events_fn)
      {
        asp.events_fn(*this, asp);
      }
    }

    Graph::think(Data::get_graph_data());
  }

  // Thinking
  {
    BENCH_SCOPED_CPU(Thinking)


    for(Aspect &asp : m_impl->aspects)
    {
      BENCH_SCOPED_CPU(AspectEarlyThink)

      if(asp.early_think_fn)
      {
        asp.early_think_fn(*this, asp);
      }
    }

    // Think
    for(Aspect &asp : m_impl->aspects)
    {
      BENCH_SCOPED_CPU(AspectThink)

      if(asp.think_fn)
      {
        asp.think_fn(*this, asp);
      }
    }

    // Late Think
    for(Aspect &asp : m_impl->aspects)
    {
      BENCH_SCOPED_CPU(AspectLateThink)

      if(asp.late_think_fn)
      {
        asp.late_think_fn(*this, asp);
      }
    }
  }

  /*
    Check to see if any aspects are ready to quit.
  */
  {
    bool should_quit = false;

    for(Aspect &asp : m_impl->aspects)
    {
      should_quit |= asp.want_to_quit;
    }

    return !should_quit;
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
