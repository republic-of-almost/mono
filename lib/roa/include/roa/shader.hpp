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
  explicit          Shader(const uint32_t id);
  explicit          Shader(const char *name, const char *filepath = nullptr);
                    ~Shader();


  // ---------------------------------------------------------- [ Inherited ] --
  
  
  const char *      get_resource_type_name() const override;
  Resource_status   get_load_status() const override;
  
  
};


} // ns


#endif // inc guard
