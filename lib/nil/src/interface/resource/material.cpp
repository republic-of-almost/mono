#include <nil/resource/material.hpp>
#include <lib/array.hpp>
#include <lib/key.hpp>
#include <lib/string_pool.hpp>
#include <lib/logging.hpp>


namespace {


lib::array<uint32_t, 128> keys(uint32_t{0});
lib::array<Nil::Resource::Material, 128> materials(Nil::Resource::Material{});


} // anon ns


namespace Nil {
namespace Resource {


// ----------------------------------------------------------------- [ Find ] --


bool
find_by_name(const char *name, Material &out)
{
  const uint32_t find_key = lib::string_pool::find(name);
  
  for(size_t i = 0; i < keys.size(); ++i)
  {
    if(keys[i] == find_key)
    {
      out = materials[i];
      return true;
    }
  }
  
  return false;
}


// ----------------------------------------------------------- [ Get / Load ] --


bool
load(const char *name, Material &in_out)
{
  const uint32_t check_key = lib::string_pool::find(name);
  
  if(check_key)
  {
    size_t index = 0;
    if(lib::key::linear_search(check_key, keys.data(), keys.size(), &index))
    {
      materials[index] = in_out;
      return true;
    }
    
    return false;
  }
  else
  {
    const uint32_t key = lib::string_pool::add(name);
    
    in_out.id = keys.size();
    
    keys.emplace_back(key);
    materials.emplace_back(in_out);
    
    return true;
  }
}


void
get(size_t *count, Material **out)
{
  *count = keys.size();
  *out = materials.data();
}


// ----------------------------------------------------------------- [ Info ] --


const char *
get_type_name(const Material &)
{
  return "Material";
}


} // ns
} // ns
