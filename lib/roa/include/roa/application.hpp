#ifndef APPLICATION_INCLUDED_410EDE8C_F4B7_498C_82F8_676D317169C4
#define APPLICATION_INCLUDED_410EDE8C_F4B7_498C_82F8_676D317169C4


#include <roa/fundamental.hpp>


namespace ROA {


using Custom_tick_fn = void(*)(uintptr_t user_data);


class Application final
{
public:


  // ----------------------------------------------------------- [ Lifetime ] --
  

  explicit        Application();
                  ~Application();
  
  void            run(Custom_tick_fn tick = nullptr, uintptr_t user_data = 0);
  

private:

  struct Impl;
  Impl *m_impl;

};


} // ns


#endif // inc guard
