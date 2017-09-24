#ifndef RESOURCE_INCLUDED_B70107CE_DD80_40F4_B6FF_E10A026783D5
#define RESOURCE_INCLUDED_B70107CE_DD80_40F4_B6FF_E10A026783D5


#include <roa/fundamental.hpp>
#include <roa/detail/resource_interface.hpp>


namespace ROA {


class Resource
{
private:

  friend ROA::Resource ROA_detail::setup_resource(const uint32_t id);

public:


  // ----------------------------------------------------------- [ Lifetime ] --


  explicit                  Resource();
  virtual                   ~Resource();
  
                            Resource(const Resource &other);
                            Resource(Resource &&other);


  // ---------------------------------------------------------- [ Operators ] --
  
  
                            operator bool() const;
  
  bool                      operator==(const Resource &other) const;
  bool                      operator!=(const Resource &other) const;
  
  Resource&                 operator=(const Resource &other);
  Resource&                 operator=(Resource &&other);
  
  
  // ---------------------------------------------------------- [ Attibutes ] --
  
  
  bool                      is_valid() const;
  uint32_t                  get_id() const;
  
  uint32_t                  get_resource_type_id() const;
  const char *              get_resource_type_name() const;
  const char *              get_instance_name() const;
  Resource_status           get_status() const;
  
  void                      load();
  
  
  // ----------------------------------------------- [ Create / Load / Find ] --
  
  
  template<typename T>
  static T
  create(const char *name)
  {
    T rsrc{};
    
    return ROA_detail::create_resource(rsrc, name);
  }
  
  template<typename T>
  static T
  load(const char *filename)
  {
    T rsrc{};
    
    return ROA_detail::load_resource(rsrc, filename);
  }
  
  template<typename T>
  static T
  find(const char *name)
  {
    T rsrc{};
    
    return ROA_detail::find_resource(rsrc, name);
  }
  
  
protected:


  mutable uint32_t m_id;

};


} // ns


#endif // inc guard
