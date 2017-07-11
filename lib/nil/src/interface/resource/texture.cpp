#include <nil/resource/texture.hpp>
#include <lib/array.hpp>
#include <lib/string_pool.hpp>
#include <lib/logging.hpp>


// ------------------------------------------------------------- [ Resource ] --


namespace {


lib::array<uint32_t, 128> keys(uint32_t{0});
lib::array<Nil::Resource::Texture, 128> textures(Nil::Resource::Texture{});


} // anon ns


namespace Nil {
namespace Resource {


// ----------------------------------------------------------------- [ Find ] --


bool
find_by_name(const char *name, Texture &out)
{
  const uint32_t find_key = lib::string_pool::find(name);
  
  for(size_t i = 0; i < keys.size(); ++i)
  {
    if(keys[i] == find_key)
    {
      out = textures[i];
      return true;
    }
  }
  
  return false;
}


// ----------------------------------------------------------- [ Get / Load ] --


bool
load(const char *name, Texture &in_out)
{
  const uint32_t check_key = lib::string_pool::find(name);
  
  if(check_key)
  {
    LOG_WARNING("Texture with this name already exists.");
    return false;
  }
  else
  {
    const uint32_t key = lib::string_pool::add(name);
    
    Texture cpy_in_out = in_out;
    
    if(in_out.status == 0)
    {
      const size_t data_size = sizeof(char) * in_out.data_size;
      char* cpy_data = (char*)malloc(data_size);
      memcpy(cpy_data, (void*)in_out.data, data_size);
      
      cpy_in_out.data = (uintptr_t)cpy_data;
    }
    
    const uint32_t new_id = keys.size();
    
    in_out.id = new_id;
    cpy_in_out.id = new_id;
    
    keys.emplace_back(key);
    textures.emplace_back(in_out);
    
    return true;
  }
}


void
get(size_t *count, Texture **in_out)
{
  *count = keys.size();
  *in_out = textures.data();
}


// ----------------------------------------------------------------- [ Info ] --


const char *
get_type_name(const Texture &)
{
  return "Texture";
}



} // ns
} // ns
