#include <roa/resource.hpp>
#include <roa/resource_status.hpp>
#include <common/context.hpp>
#include <common/resource_identifiers.hpp>
#include <nil/resource/resource.hpp>
#include <lib/assert.hpp>
#include <lib/entity.hpp>
#include <lib/optimizations.hpp>


namespace ROA {


// ------------------------------------------------------------- [ Lifetime ] --


Resource::Resource()
: m_id(0)
{
}


Resource::~Resource()
{
}


Resource::Resource(const Resource &other)
: m_id(other.m_id)
{
}


Resource::Resource(Resource &&other)
: m_id(other.m_id)
{
  other.m_id = 0;
}


// ------------------------------------------------------------ [ Operators ] --


Resource::operator bool() const
{
  return is_valid();
}


bool
Resource::operator==(const Resource &other) const
{
  return get_id() == other.get_id();
}


bool
Resource::operator!=(const Resource &other) const
{
  return get_id() != other.get_id();
}


Resource&
Resource::operator=(const Resource &other)
{
  m_id = other.m_id;
  
  return *this;
}


Resource&
Resource::operator=(Resource &&other)
{
  m_id = other.m_id;
  other.m_id = 0;
  
  return *this;
}


// ----------------------------------------------------------- [ Attributes ] --


bool
Resource::is_valid() const
{
  const uint32_t instance_id = lib::entity::instance(m_id);
  return instance_id > 0;
}


uint32_t
Resource::get_id() const
{
  const uint32_t instance_id = lib::entity::instance(m_id);
  return instance_id;
}


uint32_t
Resource::get_resource_type_id() const
{
  const uint32_t type_id = lib::entity::type(m_id);
  return type_id;
}


const char *
Resource::get_resource_type_name() const
{
  const uint32_t type_id = lib::entity::type(m_id);
  
  return ROA_detail::convert_from_type_id(type_id);
}

const char *
Resource::get_instance_name() const
{
  const uint32_t type_id     = lib::entity::type(m_id);
  const uint32_t instance_id = lib::entity::instance(m_id);
  
  switch (type_id)
  {
    case ROA_detail::Resource::AUDIO:
    {
      size_t count = 0;
      Nil_audio_src *data = nullptr;
      Nil_ctx *ctx = ROA_detail::get_ctx();
      
      nil_rsrc_audio_src_get_data(ctx, &count, &data);

      LIB_ASSERT(instance_id < count);
      
      return data[instance_id].name;
    }
    
    case ROA_detail::Resource::FONT:
    {
      size_t count = 0;
      Nil::Resource::Font *data = nullptr;
      
      Nil::Resource::get(&count, &data);
      
      LIB_ASSERT(instance_id < count);
      
      return data[instance_id].name;
    }
    
    case ROA_detail::Resource::TEXTURE:
    {
      size_t count = 0;
      Nil_texture *data = nullptr;
      Nil_ctx *ctx = ROA_detail::get_ctx();
      
      nil_rsrc_texture_get_data(ctx, &count, &data);
      
      LIB_ASSERT(instance_id < count);
      
      return data[instance_id].name;
    }
    
    case ROA_detail::Resource::MATERIAL:
    {
      size_t count = 0;
      Nil::Resource::Material *data = nullptr;
      
      Nil::Resource::get(&count, &data);
      
      LIB_ASSERT(instance_id < count);
      
      return data[instance_id].name;
    }
    
    case ROA_detail::Resource::SHADER:
    {
      size_t count = 0;
      Nil_shader *data = nullptr;
      
      nil_rsrc_shader_get_data(ROA_detail::get_ctx(), &count, &data);
      
      LIB_ASSERT(instance_id < count);
      
      return data[instance_id].name;
    }

    default:
    {
      LIB_ASSERT(false); // Missing something
      return "";
    }
  }
  
  UNREACHABLE;
  return "";
}


Resource_status
Resource::get_status() const
{
  const uint32_t type_id     = lib::entity::type(m_id);
  const uint32_t instance_id = lib::entity::instance(m_id);
  
  uint32_t status = (uint32_t)Nil::Resource::Load_status::ERROR;
  
  switch (type_id)
  {
    case ROA_detail::Resource::AUDIO:
    {
      size_t count = 0;
      Nil_audio_src *data = nullptr;
      Nil_ctx *ctx = ROA_detail::get_ctx();
      
      nil_rsrc_audio_src_get_data(ctx, &count, &data);

      
      LIB_ASSERT(instance_id < count);
      
      status = (uint32_t)data[instance_id].status;
      
      break;
    }
    
    case ROA_detail::Resource::FONT:
    {
      size_t count = 0;
      Nil::Resource::Font *data = nullptr;
      
      Nil::Resource::get(&count, &data);
      
      LIB_ASSERT(instance_id < count);
      
      status = (uint32_t)data[instance_id].status;
      
      break;
    }
    
    case ROA_detail::Resource::TEXTURE:
    {
      size_t count = 0;
      Nil_texture *data = nullptr;
      
      nil_rsrc_texture_get_data(ROA_detail::get_ctx(), &count, &data);
      
      LIB_ASSERT(instance_id < count);
      
      status = (uint32_t)data[instance_id].status;
      
      break;
    }
    
    case ROA_detail::Resource::MATERIAL:
    {
      size_t count = 0;
      Nil::Resource::Material *data = nullptr;
      
      Nil::Resource::get(&count, &data);
      
      LIB_ASSERT(instance_id < count);
      
      status = (uint32_t)data[instance_id].status;
      
      break;
    }
    
    case ROA_detail::Resource::SHADER:
    {
      size_t count = 0;
      Nil_shader *data = nullptr;
      
      nil_rsrc_shader_get_data(ROA_detail::get_ctx(), &count, &data);
      
      LIB_ASSERT(instance_id < count);
      
      status = data[instance_id].status;
      
      break;
    }

    default:
    {
      LIB_ASSERT(false); // Missing something
      status = (uint32_t)Nil::Resource::Load_status::ERROR;
      
      break;
    }
  }

  return ROA_detail::convert_from_nil((Nil::Resource::Load_status)status);
}


void
Resource::load()
{
  const uint32_t type_id     = lib::entity::type(m_id);
  const uint32_t instance_id = lib::entity::instance(m_id);
  
  switch (type_id)
  {
    case ROA_detail::Resource::AUDIO:
    {
      size_t count = 0;
      Nil_audio_src *data = nullptr;
      Nil_ctx *ctx = ROA_detail::get_ctx();
      
      nil_rsrc_audio_src_get_data(ctx, &count, &data);

      
      LIB_ASSERT(instance_id < count);
      
      if(data[instance_id].status == NIL_RSRC_STATUS_NONE)
      {
        data[instance_id].status = NIL_RSRC_STATUS_PENDING;
      }
      
      break;
    }
    
    case ROA_detail::Resource::FONT:
    {
      size_t count = 0;
      Nil::Resource::Font *data = nullptr;
      
      Nil::Resource::get(&count, &data);
      
      LIB_ASSERT(instance_id < count);
      
      if(data[instance_id].status == Nil::Resource::Load_status::NONE)
      {
        data[instance_id].status = Nil::Resource::Load_status::PENDING;
      }
      
      break;
    }
    
    case ROA_detail::Resource::TEXTURE:
    {
      size_t count = 0;
      Nil_texture *data = nullptr;
      
      nil_rsrc_texture_get_data(ROA_detail::get_ctx(), &count, &data);
      
      LIB_ASSERT(instance_id < count);
      
      if(data[instance_id].status == NIL_RSRC_STATUS_NONE)
      {
        data[instance_id].status = NIL_RSRC_STATUS_PENDING;
      }
      
      break;
    }
    
    case ROA_detail::Resource::MATERIAL:
    {
      size_t count = 0;
      Nil::Resource::Material *data = nullptr;
      
      Nil::Resource::get(&count, &data);
      
      LIB_ASSERT(instance_id < count);
      
      if(data[instance_id].status == Nil::Resource::Load_status::NONE)
      {
        data[instance_id].status = Nil::Resource::Load_status::PENDING;
      }
      
      break;
    }
    
    case ROA_detail::Resource::SHADER:
    {
      size_t count = 0;
      Nil_shader *data = nullptr;
      
      nil_rsrc_shader_get_data(ROA_detail::get_ctx(), &count, &data);
      
      LIB_ASSERT(instance_id < count);
      
      if(data[instance_id].status == NIL_RSRC_STATUS_NONE)
      {
        data[instance_id].status = NIL_RSRC_STATUS_PENDING;
      }
      
      break;
    }

    default:
    {
      LIB_ASSERT(false); // Missing something
      
      break;
    }
  }
}


} // ns
