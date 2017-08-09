#include <nil/resource/material.hpp>
#include <lib/array.hpp>
#include <lib/key.hpp>
#include <lib/string_pool.hpp>
#include <lib/logging.hpp>


namespace {


struct Material_data
{
  lib::array<uint32_t, 128> keys{uint32_t{0}};
  lib::array<Nil::Resource::Material, 128> materials{Nil::Resource::Material{}};
};


Material_data&
get_mat_data()
{
  static Material_data data;
  return data;
}


} // anon ns


namespace Nil {
namespace Resource {


// ----------------------------------------------------------------- [ Find ] --


bool
find_by_name(const char *name, Material &out)
{
  const uint32_t find_key = lib::string_pool::find(name);
  
  for(size_t i = 0; i < get_mat_data().keys.size(); ++i)
  {
    if(get_mat_data().keys[i] == find_key)
    {
      out = get_mat_data().materials[i];
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
    if(lib::key::linear_search(check_key, get_mat_data().keys.data(), get_mat_data().keys.size(), &index))
    {
      in_out.id = get_mat_data().materials[index].id;
      get_mat_data().materials[index] = in_out;
      return true;
    }
    
    return false;
  }
  else
  {
    const uint32_t key = lib::string_pool::add(name);
    
    in_out.id = get_mat_data().keys.size();
    
    get_mat_data().keys.emplace_back(key);
    get_mat_data().materials.emplace_back(in_out);
    
    return true;
  }
}


void
get(size_t *count, Material **out)
{
  *count = get_mat_data().keys.size();
  *out = get_mat_data().materials.data();
}


// ----------------------------------------------------------------- [ Info ] --


const char *
get_type_name(const Material &)
{
  return "Material";
}


} // ns
} // ns
