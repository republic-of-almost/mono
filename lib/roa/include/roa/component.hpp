#ifndef COMPONENT_INCLUDED_229A81E7_5105_497A_8195_4E5F07CCCD82
#define COMPONENT_INCLUDED_229A81E7_5105_497A_8195_4E5F07CCCD82


#include <roa/fundamental.hpp>


namespace ROA {


class Component
{
public:


  // ----------------------------------------------------------- [ Lifetime ] --

  
  explicit              Component();
  virtual               ~Component();
  
  
  // --------------------------------------------------------- [ Attributes ] --
  
  
//  virtual uint32_t      get_rtti() const;
  virtual const char *  get_name() const;
  
  
  // ---------------------------------------------------- [ Interface Hooks ] --


public:
  
  
  virtual void          on_start();
  virtual void          on_early_think();
  virtual void          on_think();
  virtual void          on_late_think();
  

};


} // ns


#endif // inc guard
