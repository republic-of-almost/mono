#include <roa/resource.hpp>
#include <roa/resource_status.hpp>
#include <common/resource_identifiers.hpp>
#include <lib/assert.hpp>
#include <lib/entity.hpp>


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
  return lib::entity::instance(m_id) > 0;
}


uint32_t
Resource::get_id() const
{
  return lib::entity::instance(m_id);
}


uint32_t
Resource::get_resource_type_id() const
{
  return ROA_detail::Resource::UNKNOWN;
}


const char *
Resource::get_resource_type_name() const
{
  const uint32_t type_id = lib::entity::type(m_id);
  
  return ROA_detail::resource_names[type_id];
}


} // ns
