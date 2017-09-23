#include <roa/shader.hpp>
#include <roa/resource_status.hpp>
#include <nil/resource/shader.hpp>


namespace ROA {


// ------------------------------------------------------------- [ Lifetime ] --


Shader::Shader()
: Resource()
{
}


Shader::~Shader()
{
}


// ------------------------------------------------------------ [ Inherited ] --


Resource_status
Shader::get_load_status() const
{
  return Resource_status::PENDING;
}


} // ns
