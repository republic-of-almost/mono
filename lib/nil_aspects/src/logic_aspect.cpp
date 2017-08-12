#include <aspect/logic_aspect.hpp>
#include <lib/utilities.hpp>
#include <nil/aspect.hpp>
#include <nil/data/data.hpp>
#include <lib/bench.hpp>


#ifdef IMGUI_DEVELOPER_SUPPORT


// --------------------------------------------------- [ Logic Aspect IMGUI ] --


namespace {


#ifndef NIMGUI
inline void
logic_aspect_debug_menu(uintptr_t user_data)
{
}


inline void
logic_aspect_debug_window(uintptr_t user_data)
{
}
#endif

} // anon ns
#endif


namespace Nil_ext {
namespace Logic_aspect {


// -------------------------------------------- [ Logic Aspect Impl Startup ] --


void
start_up(Nil::Engine &engine, Nil::Aspect &aspect)
{
  Data *self = reinterpret_cast<Data*>(aspect.user_data);
  LIB_ASSERT(self);

  aspect.data_types = 0;
  aspect.data_types |= Nil::Data::get_type_id(Nil::Data::Logic{});
}


// --------------------------------------------- [ Logic Aspect Impl Events ] --


void
events(Nil::Engine &engine, Nil::Aspect &aspect)
{
  Data *self = reinterpret_cast<Data*>(aspect.user_data);
  LIB_ASSERT(self);

  // Logic
  {
    size_t            count = 0;
    Nil::Data::Logic *data  = nullptr;
    Nil::Node        *node  = nullptr;
    
    Nil::Data::events(Nil::Data::Event::ADDED, &count, &data, &node);
    
    for(size_t i = 0; i < count; ++i)
    {
      if(data[i].logic_id == 1)
      {
        bool exists = false;
        
        for(auto n : self->logic_nodes)
        {
          if(n == node[i])
          {
            exists = true;
          }
        }
        
        if(!exists)
        {
          self->logic_nodes.emplace_back(node[i]);
          self->update.emplace_back(data[i].think_01);
          self->user_data.emplace_back(data[i].user_data);
        }
      }
    }
  }
  
  Nil::Task::cpu_task(
    Nil::Task::CPU::EARLY_THINK,
    (uintptr_t)self,
    early_think
  );
}


// ---------------------------------------- [ Logic Aspect Impl Early Think ] --


void
early_think(Nil::Engine &engine, uintptr_t user_data)
{
  Data *self = reinterpret_cast<Data*>(user_data);
  LIB_ASSERT(self);
  
  BENCH_SCOPED_CPU(Logic_EarktThink)

  for(size_t i = 0; i < self->logic_nodes.size(); ++i)
  {
    self->update[i](self->logic_nodes[i].get_id(), self->user_data[i]);
  }
}


} // ns
} // ns
