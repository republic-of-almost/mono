#include <nil/nil.hpp>
#include <nil/node.hpp>
#include <nil/data/data.hpp>
#include <aspect/aspects.hpp>
#include <GL/gl3w.h>
#include <renderable.hpp>
#include <lib/array.hpp>

Nil::Node window;

lib::array<edgeCamera> cameras;
lib::array<edgeRenderable> renderables;

int
main()
{
  Nil::Engine nil_engine;
  Nil_ext::load_aspects(nil_engine);

  // Window
  {
    window.set_name("Window");

    Nil::Data::Window win{};
    constexpr char title[] = "App";
    strcat(win.title, title);

    Nil::Data::set(window, win);
  }

  nil_engine.run(); // Init graphics etc.
  nil_engine.run(); // Init graphics etc.
  nil_engine.run(); // Init graphics etc.

  edgeInit();

  cameras.emplace_back();
  renderables.emplace_back();

  while(nil_engine.run())
  {
    if(!cameras.empty() && !renderables.empty())
    {
      edgeExecute(
        cameras.data(),
        cameras.size(),
        renderables.data(),
        renderables.size()
      );
    }
  }

  return 0;
}
