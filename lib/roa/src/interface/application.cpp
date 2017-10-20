#include <roa/application.hpp>
#include <common/common.hpp>
#include <common/context.hpp>
#include <nil/nil.hpp>
#include <nil/node.hpp>
#include <nil/data/data.hpp>
#include <aspect/aspects.hpp>
#include <component/component_engine.hpp>


/* ---------------------------------------------- [ Application Internal ] -- */


static void
roa_internal_application_tick(uintptr_t user_data, roa_custom_tick_fn cb)
{
  Nil_ctx *ctx = ROA_detail::get_ctx();

  while(nil_ctx_think(ctx))
  {
    /*
      Components
      --
      Components live in ROA rather than Nil.
      So updates and so forth will need to be called now.
      Components are single threaded right now.
    */
    {
      ROA_detail::components_start_hooks();
      ROA_detail::components_early_think_hooks();
      ROA_detail::components_think_hooks();
      ROA_detail::components_late_think_hooks();
    }
    
    
    /*
      Custom Callback
      --
      The user may specifiy a callback we do it before we start another frame.
    */
    if(cb)
    {
      cb(user_data);
    }
  }
}


/* ---------------------------------------------- [ Application Lifetime ] -- */


void
roa_application_init()
{
  Nil_ctx *ctx = ROA_detail::get_ctx();

  Nil_ext::load_aspects(ctx);

  Nil::Node app_node = ROA_detail::get_application_node();
  app_node.set_name("Application");

  Nil::Data::Window win{};
  constexpr char title[] = "ROA";
  strcat(win.title, title);

  Nil::Data::set(app_node, win);
  Nil::Data::set(app_node, Nil::Data::Mouse{});
  Nil::Data::set(app_node, Nil::Data::Keyboard{});
}


void
roa_application_destroy()
{
}


void
roa_application_run()
{
  roa_internal_application_tick(NULL, NULL);
}


void
roa_application_run_with_tick(uintptr_t user_data, roa_custom_tick_fn cb)
{
  roa_internal_application_tick(user_data, cb);
}

