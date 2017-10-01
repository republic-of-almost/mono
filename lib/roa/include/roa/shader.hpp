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
  
  
  // --------------------------------------------------------- [ Attributes ] --
  
  
  void              set_shader_type(const Shader_type type);
  Shader_type       get_shader_type() const;
  
  bool              set_vertex_shader_code(const char *src);
  const char *      get_vertex_shader_code() const;
  
  bool              set_geometry_shader_code(const char *src);
  const char *      get_geometry_shader_code() const;
  
  bool              set_fragment_shader_code(const char *src);
  const char *      get_fragment_shader_code() const;
  
};


} // ns


#endif // inc guard
