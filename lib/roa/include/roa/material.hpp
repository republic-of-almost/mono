#ifndef MATERIAL_INCLUDED_1BC55ED8_5498_4064_BB1B_CFA9D585FED3
#define MATERIAL_INCLUDED_1BC55ED8_5498_4064_BB1B_CFA9D585FED3


#include <roa/fundamental.hpp>
#include <roa/resource.hpp>


namespace ROA {


class Material final : public Resource
{
public:


  // ----------------------------------------------------------- [ Lifetime ] --
  

  explicit          Material();
  explicit          Material(const uint32_t id);
  explicit          Material(const char *name);
  explicit          Material(const char *name, const Color &col);
                    ~Material();
  
  
  // --------------------------------------------------------- [ Attributes ] --
  
  
  Color             get_color() const;
  void              set_color(const Color &col);
  
  Shader            get_shader() const;
  void              set_shader(const Shader &shader);
  
  
  // ---------------------------------------------------------- [ Inherited ] --
  
  
  const char *      get_resource_type_name() const override;
  Resource_status   get_load_status() const override;


};


} // ns


#endif // inc guard
