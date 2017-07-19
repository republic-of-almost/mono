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

#include <roa/object.hpp>


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

  Nil::Engine nil_engine;
  Nil_ext::load_aspects(nil_engine);
  
  ROA::Object obj;
  obj.set_name("FooFoo");
  
  Game_data::setup();
  Game_data::load_assets();

  Game::Actor actor;
  Game::setup(&actor);
  
  Game::Static_objects objs;
  Game::setup(&objs);
  
  nil_engine.run();
  nil_engine.run();
  
  nau_init();
  nau_gl3_init();
  
  /*
    Run Game
  */
  #ifdef __EMSCRIPTEN__
  emscripten_set_main_loop(app_tick, -1, 1);
  #else
  while(nil_engine.run())
  {
    app_tick();
    
    // Nau Test
    {
      nau_new_frame();
    
      static float flt = 1.f;
      
      nau_begin("Foo");
      nau_float("test", &flt);
      nau_end();
      
      size_t count = 0;
      Nau_renderable *renderables = nullptr;
      
      nau_render_data(&renderables, &count);
//      nau_gl3_render(renderables, count);
    }
  }
  #endif
  
  nau_end();

  return 0;
}
