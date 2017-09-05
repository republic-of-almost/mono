#include <roa/application.hpp>
#include <common/common.hpp>
#include <nil/nil.hpp>
#include <nil/node.hpp>
#include <nil/data/data.hpp>
#include <aspect/aspects.hpp>
#include <component/component_engine.hpp>


namespace ROA {


struct Application::Impl
{
  
};


Application::Application()
: m_impl{new Impl}
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
  LIB_ASSERT(m_impl);
  
  if(m_impl)
  {
    delete m_impl;
  }
}


void
Application::run(Custom_tick_fn tick, uintptr_t user_data)
{
  while(ROA_detail::get_engine().run())
  {
    /*
      Components
      --
      Components live in ROA rather than Nil.
      So updates and so forth will need to be called now.
      Components are single threaded right now.
    */
    {
      ROA_detail::components_early_think_hooks();
      ROA_detail::components_think_hooks();
      ROA_detail::components_late_think_hooks();
    }
    
    
    /*
      Custom Callback
      --
      The user may specifiy a callback we do it before we start another frame.
    */
    if(tick)
    {
      tick(user_data);
    }
  }
}


} // ns
