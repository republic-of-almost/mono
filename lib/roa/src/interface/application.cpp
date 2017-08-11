#include <roa/application.hpp>
#include <common/common.hpp>
#include <nil/nil.hpp>
#include <nil/node.hpp>
#include <nil/data/data.hpp>
#include <aspect/aspects.hpp>


namespace ROA {


Application::Application()
{
  Nil_ext::load_aspects(ROA_detail::get_engine());

  Nil::Node app_node = ROA_detail::get_application_node();
  app_node.set_name("Application");

  Nil::Data::Window win{};
  constexpr char title[] = "ROA";
  strcat(win.title, title);

  Nil::Data::set(app_node, win);
  Nil::Data::set(app_node, Nil::Data::Mouse{});
  Nil::Data::set(app_node, Nil::Data::Keyboard{});
}


Application::~Application()
{
}


void
Application::run(Custom_tick_fn tick, uintptr_t user_data)
{
  while(ROA_detail::get_engine().run())
  {
    if(tick)
    {
      tick(user_data);
    }
  }
}


} // ns
