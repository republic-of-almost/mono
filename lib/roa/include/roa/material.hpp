#ifndef MATERIAL_INCLUDED_1BC55ED8_5498_4064_BB1B_CFA9D585FED3
#define MATERIAL_INCLUDED_1BC55ED8_5498_4064_BB1B_CFA9D585FED3


#include <roa/fundamental.hpp>
#include <roa/resource.hpp>


namespace ROA {


class Material final : public Resource
{
public:


  // ----------------------------------------------------------- [ Lifetime ] --
  
  
  explicit            Material();
  virtual             ~Material();
  
  
  // --------------------------------------------------------- [ Attributes ] --
  
  
  Shader              get_shader() const;
  void                set_shader(const Shader &shd);
  
  Color               get_color() const;
  void                set_color(const Color &col);
  
  
  // ---------------------------------------------------------- [ Inherited ] --
  
  
  const char *        get_instance_name() const;
  Resource_status     get_load_status() const;


};


} // ns


#endif // inc guard
