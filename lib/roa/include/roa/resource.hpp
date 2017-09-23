#ifndef RESOURCE_INCLUDED_B70107CE_DD80_40F4_B6FF_E10A026783D5
#define RESOURCE_INCLUDED_B70107CE_DD80_40F4_B6FF_E10A026783D5


#include <roa/fundamental.hpp>


namespace ROA {


class Resource
{
protected:


  // ----------------------------------------------------------- [ Lifetime ] --
  

  explicit                  Resource(const uint32_t id);
  explicit                  Resource();
  virtual                   ~Resource();


public:
  
  
  // ---------------------------------------------------------- [ Attibutes ] --
  
  
  inline uint32_t           get_id() const { return m_id; }
  bool                      is_valid() const;
  
  
  // ---------------------------------------------------------- [ Inherited ] --
  
  
  virtual const char *      get_resource_type_name() const;
  virtual Resource_status   get_load_status() const;
  
  
protected:

  uint32_t                  m_id;

};


} // ns


#endif // inc guard
