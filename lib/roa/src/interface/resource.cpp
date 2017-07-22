#include <roa/resource.hpp>


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


} // ns
