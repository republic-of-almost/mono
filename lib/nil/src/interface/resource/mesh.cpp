#include <nil/resource/mesh.hpp>
#include <lib/array.hpp>
#include <lib/string_pool.hpp>
#include <lib/logging.hpp>


namespace {


lib::array<uint32_t> keys(uint32_t{0});
lib::array<Nil::Resource::Mesh> meshes(Nil::Resource::Mesh{});


} // anon ns


namespace Nil {
namespace Resource {


// ----------------------------------------------------------------- [ Find ] --


void
find_by_name(const char *name, Mesh &out)
{
  const uint32_t find_key = lib::string_pool::find(name);
  
  for(size_t i = 0; i < keys.size(); ++i)
  {
    if(keys[i] == find_key)
    {
      out = meshes[i];
      return;
    }
  }
}


// ----------------------------------------------------------- [ Get / Load ] --


bool
load(const char *name, Mesh &in_out)
{
  const uint32_t check_key = lib::string_pool::find(name);
  
  if(check_key)
  {
    LOG_WARNING("Mesh with this name already exists.");
    return false;
  }
  else
  {
    const uint32_t key = lib::string_pool::add(name);
    
    in_out.id = keys.size();
    
    keys.emplace_back(key);
    meshes.emplace_back(in_out);
    
    return true;
  }
}


void
get(size_t *count, Mesh **out)
{
  *count = keys.size();
  *out = meshes.data();
}


} // ns
} // ns
