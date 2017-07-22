#include <roa/collection.hpp>
#include <roa/object.hpp>
#include <vector>


namespace ROA {


// ---------------------------------------------------------------- [ Data ] --


struct Collection::Impl
{
  std::vector<ROA::Object> objs;
};


// ------------------------------------------------------------- [ Lifetime ] --


Collection::Collection()
: Collection(nullptr, 0)
{
}


Collection::Collection(const Object *obj, const size_t count)
: m_impl(new Impl)
{
  m_impl->objs.resize(count);
  
  for(size_t i = 0; i < count; ++i)
  {
    m_impl->objs[i] = obj[i];
  }
}


Collection::~Collection()
{
  delete m_impl;
}

Collection::Collection(const Collection &other)
: Collection(other.data(), other.size())
{
  
}


Collection::Collection(Collection &&other)
: Collection(other.data(), other.size())
{
  other.m_impl->objs.clear();
}


Collection&
Collection::operator=(const Collection &other)
{
  m_impl->objs = other.m_impl->objs;
  return *this;
}


Collection&
Collection::operator=(Collection &&other)
{
  m_impl->objs = other.m_impl->objs;
  return *this;
}
  
  
// --------------------------------------------------------------- [ Access ] --


const Object*
Collection::begin() const
{
  return &m_impl->objs[0];
}


Object*
Collection::begin()
{
  return &m_impl->objs[0];
}


const Object*
Collection::end() const
{
  return &m_impl->objs[m_impl->objs.size()];
}


Object*
Collection::end()
{
  return &m_impl->objs[m_impl->objs.size()];
}


const Object*
Collection::data() const
{
  return m_impl->objs.data();
}


Object*
Collection::data()
{
  return m_impl->objs.data();
}
  
  
// ----------------------------------------------------------- [ Attributes ] --
  
  
bool
Collection::empty() const
{
  return m_impl->objs.empty();
}


size_t
Collection::size() const
{
  return m_impl->objs.size();
}
  

} // ns
