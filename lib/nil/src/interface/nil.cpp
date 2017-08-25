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
#include <tinydir/tinydir.h>
#include <lib/platform.hpp>
#include <lib/directory.hpp>
#include <lib/string.hpp>


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
  
  // -- Scan Asset Directories -- //
  // -- And Queue up asset loading -- //
  {
    // Textures //
    {
      char path[2048]{};
      strcat(path, lib::dir::exe_path());
      
      #ifndef LIB_PLATFORM_MAC
      strcat(path, "assets/texture");
      #else
      strcat(path, "../Resources/assets/texture");
      #endif

      _tinydir_char_t wpath[2048]{};
      
      #ifdef LIB_PLATFORM_WIN
      mbstowcs(wpath, path, 2048);
      #else
      memcpy(wpath, path, sizeof(path));
      #endif

      _tinydir_char_t dir_path[2048]{};
      _tinydir_strcat(dir_path, wpath);

      tinydir_dir dir;
      tinydir_open(&dir, dir_path);

      while (dir.has_next)
      {
        tinydir_file file;
        tinydir_readfile(&dir, &file);
        
        // We only load up png's right now.
        _tinydir_char_t *ext = TINYDIR_STRING("png");

        if (!file.is_dir)
        {
          if(_tinydir_strcmp(ext, file.extension) == 0)
          {
            char c_name[2048]{};
            
            #ifdef LIB_PLATFORM_WIN
            wcstombs(c_name, file.path, _tinydir_strlen(file.path));
            #else
            memcpy(c_name, file.path, _tinydir_strlen(file.path));
            #endif
          
            int i = 0;

            char load_name[2048]{};
            lib::string::filename_from_path(c_name, load_name, sizeof(load_name), true);

            Nil::Resource::Texture tex{};
            tex.name      = load_name;
            tex.data      = (uintptr_t)c_name;
            tex.data_size = strlen(c_name) + 1;
            tex.data_type = Nil::Resource::Texture::FILENAME;

            Nil::Resource::load(tex);
          }
        }

        tinydir_next(&dir);
      }

      tinydir_close(&dir);
    }

    
    // Meshes //
    {
      int i = 0;
    }
  }
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
