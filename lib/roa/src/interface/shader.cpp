#include <roa/shader.hpp>
#include <roa/resource_status.hpp>
#include <common/resource_identifiers.hpp>
#include <nil/resource/shader.hpp>
#include <lib/assert.hpp>
#include <lib/entity.hpp>
#include <lib/logging.hpp>


namespace ROA {


// ------------------------------------------------------------- [ Lifetime ] --


Shader::Shader()
: Resource()
{
}


Shader::~Shader()
{
}


// ----------------------------------------------------------- [ Attributes ] --


void
Shader::set_shader_type(const Shader_type type)
{
  if(m_id)
  {
    const uint32_t index = lib::entity::instance(m_id);

    nil_rsrc_shader_set_type(index, ROA_detail::convert_from_roa(type));
  }
  else
  {
    LOG_ERROR("Invalid Shader")
  }
}


Shader_type
Shader::get_shader_type() const
{
  if(m_id)
  {
    const uint32_t index = lib::entity::instance(m_id);
    Nil_shader_type type = nil_rsrc_shader_get_type(index);
    
    return ROA_detail::convert_from_nil(type);
  }
  else
  {
    LOG_ERROR("Invalid Shader")
  }
  
  return Shader_type::NONE;
}

  
bool
Shader::set_vertex_shader_code(const char *src)
{
  if(m_id)
  {
    const uint32_t index = lib::entity::instance(m_id);
    
    return nil_rsrc_shader_set_vs_src(index, src);
  }
  else
  {
    LOG_ERROR("Invalid Shader");
  }
  
  return false;
}


const char *
Shader::get_vertex_shader_code() const
{
  if(m_id)
  {
    const uint32_t index = lib::entity::instance(m_id);
    
    return nil_rsrc_shader_get_vs_src(index);
  }
  else
  {
    LOG_ERROR("Invalid Shader");
  }
  
  return "";
}


bool
Shader::set_geometry_shader_code(const char *src)
{
  if(m_id)
  {
    const uint32_t index = lib::entity::instance(m_id);
    
    return nil_rsrc_shader_set_gs_src(index, src);
  }
  else
  {
    LOG_ERROR("Invalid Shader");
  }
  
  return false;
}


const char *
Shader::get_geometry_shader_code() const
{
  if(m_id)
  {
    const uint32_t index = lib::entity::instance(m_id);
    
    return nil_rsrc_shader_get_gs_src(index);
  }
  else
  {
    LOG_ERROR("Invalid Shader");
  }
  
  return "";
}


bool
Shader::set_fragment_shader_code(const char *src)
{
  if(m_id)
  {
    const uint32_t index = lib::entity::instance(m_id);
    
    return nil_rsrc_shader_set_fs_src(index, src);
  }
  else
  {
    LOG_ERROR("Invalid Shader");
  }
  
  return false;
}


const char *
Shader::get_fragment_shader_code() const
{
  if(m_id)
  {
    const uint32_t index = lib::entity::instance(m_id);
    
    return nil_rsrc_shader_get_fs_src(index);
  }
  else
  {
    LOG_ERROR("Invalid Shader");
  }
  
  return "";
}



} // ns
