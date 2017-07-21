#include <nil/nil.hpp>
#include <aspect/aspects.hpp>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

#include <lib/logging.hpp>
#include <lib/string.hpp>

#include <game/static_objects.hpp>
#include <game/actor.hpp>

#include <data/data.hpp>

#include <assert.h>
#include <stddef.h>
#include <string.h>

#include <lib/memory.hpp>

#include <nau/nau.h>
#include <nau/renderers/opengl3.h>

#include <roa/roa.hpp>


void
app_tick()
{
  #ifdef __EMSCRIPTEN__
  nil_engine.run();
  #endif
}



int
main()
{
  lib::mem::alloc<float, lib::mem::tag::NUMBER, 256>();

  ROA::Application app;

  Game_data::setup();
  Game_data::load_assets();

  Game::Actor actor;
  Game::setup(&actor);
  
  Game::Static_objects objs;
  Game::setup(&objs);
  
//  nau_init();
//  nau_gl3_init();
  
  /*
    Run Game
  */
  app.run([](uintptr_t user_data)
    // Nau Test
    {
//      nau_new_frame();
    
//      static float flt = 1.f;
      
//      nau_begin("Foo");
//      nau_float("test", &flt);
//      nau_end();
//      
//      size_t count = 0;
//      Nau_renderable *renderables = nullptr;
      
//      nau_render_data(&renderables, &count);
//      nau_gl3_render(renderables, count);
    },
    0
  );

//  nau_end();

  return 0;
}
