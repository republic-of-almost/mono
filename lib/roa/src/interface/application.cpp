#include <roa/application.hpp>
#include <nil/nil.hpp>
#include <nil/node.hpp>
#include <nil/data/data.hpp>
#include <aspect/aspects.hpp>


namespace ROA {


struct Application::Impl
{
  Nil::Engine nil_engine;
  Nil::Node app_window;
};


Application::Application()
: m_impl(new Impl)
{
  Nil_ext::load_aspects(m_impl->nil_engine);
  
  m_impl->app_window.set_name("Application");
  
  Nil::Data::Window win{};
  constexpr char title[] = "ROA";
  strcat(win.title, title);
  
  Nil::Data::set(m_impl->app_window, win);
  Nil::Data::set(m_impl->app_window, Nil::Data::Mouse{});
  Nil::Data::set(m_impl->app_window, Nil::Data::Keyboard{});
}


Application::~Application()
{
  delete m_impl;
}


void
Application::run(Custom_tick_fn tick, uintptr_t user_data)
{
  while(m_impl->nil_engine.run())
  {
    if(tick)
    {
      tick(user_data);
    }
  }
}



} // ns
