#ifndef MATERIAL_INCLUDED_1BC55ED8_5498_4064_BB1B_CFA9D585FED3
#define MATERIAL_INCLUDED_1BC55ED8_5498_4064_BB1B_CFA9D585FED3


#include <roa/fundamental.hpp>


namespace ROA {


class Material final
{
public:


  // ----------------------------------------------------------- [ Lifetime ] --


  explicit          Material(const char *name);
  
  
  // ---------------------------------------------------------- [ Attibutes ] --


private:

  uint32_t          m_id;

};


} // ns


#endif // inc guard
