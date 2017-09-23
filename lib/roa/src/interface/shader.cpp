#include <roa/shader.hpp>
#include <roa/resource_status.hpp>
#include <nil/resource/shader.hpp>


namespace ROA {


// ------------------------------------------------------------- [ Lifetime ] --


Shader::Shader()
: Resource(0)
{
}


Shader::Shader(const uint32_t id)
: Resource(id)
{
  // -- Check given id is valid -- //
}


Shader::Shader(const char *name, const char *filepath)
{
  // -- Param Check -- //
  {
    
  }
}


// ------------------------------------------------------------ [ Inherited ] --
  
  
const char *
Resource::get_resource_type_name() const
{
  return "Shader Resource";
}


Resource_status
Shader::get_load_status() const
{
  return Resource_status::PENDING;
}


} // ns
