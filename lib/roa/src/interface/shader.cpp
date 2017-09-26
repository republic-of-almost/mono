#include <roa/shader.hpp>
#include <roa/resource_status.hpp>
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
  
  
bool
Shader::set_vertex_shader_code(const char *src)
{
  if(m_id)
  {
    Nil::Resource::Shader shd;
    const uint32_t index = lib::entity::instance(m_id);
    
    if(Nil::Resource::find_by_index(index, &shd))
    {
      if(shd.status == Nil::Resource::Load_status::NONE)
      {
        shd.vs_code = src;
        return Nil::Resource::load(shd);
      }
      else
      {
        LOG_ERROR("Can't set shader its already loaded");
      }
    }
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
    Nil::Resource::Shader shd;
    const uint32_t index = lib::entity::instance(m_id);
    
    if(Nil::Resource::find_by_index(index, &shd))
    {
      return shd.vs_code;
    }
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
    Nil::Resource::Shader shd;
    const uint32_t index = lib::entity::instance(m_id);
    
    if(Nil::Resource::find_by_index(index, &shd))
    {
      if(shd.status == Nil::Resource::Load_status::NONE)
      {
        shd.gs_code = src;
        return Nil::Resource::load(shd);
      }
      else
      {
        LOG_ERROR("Can't set shader its already loaded");
      }
    }
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
    Nil::Resource::Shader shd;
    const uint32_t index = lib::entity::instance(m_id);
    
    if(Nil::Resource::find_by_index(index, &shd))
    {
      return shd.gs_code;
    }
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
    Nil::Resource::Shader shd;
    const uint32_t index = lib::entity::instance(m_id);
    
    if(Nil::Resource::find_by_index(index, &shd))
    {
      if(shd.status == Nil::Resource::Load_status::NONE)
      {
        shd.fs_code = src;
        return Nil::Resource::load(shd);
      }
      else
      {
        LOG_ERROR("Can't set shader its already loaded");
      }
    }
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
    Nil::Resource::Shader shd;
    const uint32_t index = lib::entity::instance(m_id);
    
    if(Nil::Resource::find_by_index(index, &shd))
    {
      return shd.fs_code;
    }
  }
  else
  {
    LOG_ERROR("Invalid Shader");
  }
  
  return "";
}



} // ns
