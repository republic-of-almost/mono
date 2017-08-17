#ifndef LOGIC_INCLUDED_E2CDB8D6_BCF0_409F_953C_DB0C8723535B
#define LOGIC_INCLUDED_E2CDB8D6_BCF0_409F_953C_DB0C8723535B


#include <roa/fundamental.hpp>
#include <roa/object.hpp>


namespace ROA {


using Logic_update_func = void(*)(ROA::Object obj);
using Logic_message_func = void(*)(const uint32_t id, uintptr_t data);


class Logic final : public Object
{
public:


  // ----------------------------------------------------------- [ Lifetime ] --
  

  explicit        Logic();
                  ~Logic();
  
  
  // --------------------------------------------------------- [ Attributes ] --
  
  
  void            set_update_func(Logic_update_func update_fn);
  void            set_message_func(Logic_message_func msg_fn);
  
  void            send_message(const uint32_t id, uintptr_t data);
  

};


} // ns


#endif // inc guard
