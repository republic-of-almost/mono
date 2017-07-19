#ifndef RESOURCE_INCLUDED_B70107CE_DD80_40F4_B6FF_E10A026783D5
#define RESOURCE_INCLUDED_B70107CE_DD80_40F4_B6FF_E10A026783D5


#include <roa/fundamental.hpp>


namespace ROA {


class Resource
{
public:


  // ----------------------------------------------------------- [ Lifetime ] --
  
  
  explicit            Resource();
                      ~Resource();
  
  // ---------------------------------------------------------- [ Attibutes ] --
  
  
  inline uint32_t     get_id() const { return m_id; }
  
  
protected:

  uint32_t            m_id;

};


} // ns


#endif // inc guard
