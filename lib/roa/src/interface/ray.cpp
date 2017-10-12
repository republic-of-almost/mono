#include <roa/ray.hpp>


namespace ROA {


Ray::Ray()
  : Ray(Vector3(0,0,0), Vector3(0,0,0))
{
}


Ray::Ray(const Vector3 &start, const Vector3 &end)
: m_start(start)
, m_end(end)
{
}


// ----------------------------------------------------------- [ Attributes ] --
  
  
Vector3
Ray::get_start() const
{
  return m_start;
}


Vector3
Ray::get_end() const
{
  return m_end;
}


bool
Ray::is_valid() const
{
  const bool start_zero = m_start.is_zero();
  const bool end_zero = m_end.is_zero();
  
  return start_zero && end_zero;
}

} // ns
