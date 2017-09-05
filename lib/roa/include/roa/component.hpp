#ifndef COMPONENT_INCLUDED_229A81E7_5105_497A_8195_4E5F07CCCD82
#define COMPONENT_INCLUDED_229A81E7_5105_497A_8195_4E5F07CCCD82


#include <roa/fundamental.hpp>
#include <roa/object.hpp>


#define ROA_COMPONENT_RTTI(name) \
static constexpr uint32_t get_rtti() { return __COUNTER__ + 1; } \
static const char* get_name() { return #name; }


namespace ROA {



class Component
{


  friend class ROA::Object;

public:


  // ----------------------------------------------------------- [ Lifetime ] --

  
  explicit              Component();
  virtual               ~Component();
  
  
  // --------------------------------------------------------- [ Attributes ] --
  
  
//  virtual uint32_t      get_rtti() const;
//  virtual const char *  get_name() const;
  Object                get_object() const;
  
  
  // ---------------------------------------------------- [ Interface Hooks ] --


public:
  
  
  virtual void          on_start();
  virtual void          on_early_think();
  virtual void          on_think();
  virtual void          on_late_think();
  virtual void          on_destroy();
  
  
private:

  ROA::Object m_obj{nullptr};

};


} // ns


#endif // inc guard
