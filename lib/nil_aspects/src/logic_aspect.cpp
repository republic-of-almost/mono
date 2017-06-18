#include <aspect/logic_aspect.hpp>
#include <lib/utilities.hpp>
#include <nil/aspect.hpp>
#include <nil/data/data.hpp>
#include <nil/node_event.hpp>


#ifndef NBENCHMARK
#include <remotery/Remotery.h>
#endif


#ifdef IMGUI_DEVELOPER_SUPPORT


// --------------------------------------------------- [ Logic Aspect IMGUI ] --


namespace {


#ifndef NIMGUI
inline void
logic_aspect_debug_menu(uintptr_t user_data)
{
  Nil_ext::ROV_Aspect::Data *self(
    reinterpret_cast<Nil_ext::ROV_Aspect::Data*>(user_data)
  );

  if(ImGui::BeginMenu("Logic"))
  {
    ImGui::EndMenu();
  }

  LIB_ASSERT(self);
}


inline void
logic_aspect_debug_window(uintptr_t user_data)
{
  Nil_ext::ROV_Aspect::Data *self(
    reinterpret_cast<Nil_ext::ROV_Aspect::Data*>(user_data)
  );
  LIB_ASSERT(self);
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
events(Nil::Engine &engine, Nil::Aspect &aspect, Nil::Event_list &event_list)
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
}


// ---------------------------------------- [ Logic Aspect Impl Early Think ] --


void
early_think(Nil::Engine &engine, Nil::Aspect &aspect)
{
  Data *self = reinterpret_cast<Data*>(aspect.user_data);
  LIB_ASSERT(self);
  
  rmt_ScopedCPUSample(Logic_EarktThink, 0);

  for(size_t i = 0; i < self->logic_nodes.size(); ++i)
  {
    self->update[i](self->logic_nodes[i], self->user_data[i]);
  }
}


// ----------------------------------------- [ Logic Aspect Impl Late Think ] --


void
think(Nil::Engine &engine, Nil::Aspect &aspect)
{

}


} // ns
} // ns
