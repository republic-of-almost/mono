#include <roa/resource.hpp>
#include <roa/resource_status.hpp>
#include <lib/assert.hpp>


namespace ROA {


// ------------------------------------------------------------- [ Lifetime ] --


Resource::Resource(const uint32_t id)
: m_id(id)
{
}


Resource::Resource()
: m_id(0)
{
}


Resource::~Resource()
{
}


// ----------------------------------------------------------- [ Attributes ] --


bool
Resource::is_valid() const
{
  return m_id > 0;
}


// ------------------------------------------------------------ [ Inherited ] --


const char *
Resource::get_resource_type_name() const
{
  LIB_ASSERT(false);
  return "Unknown Resource";
}


Resource_status
Resource::get_status() const
{
  LIB_ASSERT(false);
  return Resource_status::ERROR;
}


} // ns
