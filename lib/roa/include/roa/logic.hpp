#ifndef LOGIC_INCLUDED_E2CDB8D6_BCF0_409F_953C_DB0C8723535B
#define LOGIC_INCLUDED_E2CDB8D6_BCF0_409F_953C_DB0C8723535B


#include <roa/fundamental.hpp>
#include <roa/object.hpp>


namespace ROA {


using Logic_update_func = void(*)(ROA::Object obj);


class Logic final : public Object
{
public:


  // ----------------------------------------------------------- [ Lifetime ] --
  

  explicit        Logic();
                  ~Logic();
  
  
  // --------------------------------------------------------- [ Attributes ] --
  
  
  void            update_func(Logic_update_func update_fn);
  

};


} // ns


#endif // inc guard
