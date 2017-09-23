#ifndef SHADER_INCLUDED_72F4AAFA_8511_4E31_B987_90951F83344A
#define SHADER_INCLUDED_72F4AAFA_8511_4E31_B987_90951F83344A


#include <roa/fundamental.hpp>
#include <roa/resource.hpp>


namespace ROA {


class Shader final : public Resource
{
public:


  // ----------------------------------------------------------- [ Lifetime ] --
  
  
  explicit          Shader();
  virtual           ~Shader();


  // ---------------------------------------------------------- [ Inherited ] --
  
  
  Resource_status   get_load_status() const;
  
  
};


} // ns


#endif // inc guard
