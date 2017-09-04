#include <roa/component.hpp>
#include <lib/assert.hpp>


namespace ROA {


// ------------------------------------------------------------- [ Lifetime ] --


Component::Component()
{
}


Component::~Component()
{
}



// ----------------------------------------------------------- [ Attributes ] --


//uint32_t
//Component::get_rtti() const
//{
//  // Component should override this method.
//  LIB_ASSERT(false);
//  
//  return 0;
//}



const char *
Component::get_name() const
{
  // Component should override this method.
  LIB_ASSERT(false);
  
  return "Unknown Component";
}


// ------------------------------------------------------ [ Interface Hooks ] --


void
Component::on_start()
{
  /* Leave Empty */
  /* Maybe in future calling this will remove this hook from being called */
}


void
Component::on_early_think()
{
  /* Leave Empty */
  /* Maybe in future calling this will remove this hook from being called */
}


void
Component::on_think()
{
  /* Leave Empty */
  /* Maybe in future calling this will remove this hook from being called */
}


void
Component::on_late_think()
{
  /* Leave Empty */
  /* Maybe in future calling this will remove this hook from being called */
}


} // ns
