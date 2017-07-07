#include <nil/resource/mesh.hpp>
#include <lib/array.hpp>
#include <lib/string_pool.hpp>
#include <lib/logging.hpp>
#include <stdio.h>


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
    const uint32_t key = keys[i];
  
    if(key == find_key)
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
    Mesh cpy_in_out = in_out;
    
    if(in_out.status == 0)
//    if(false)
    {
      // Copy all the mesh data.
      {
        if(in_out.position_vec3)
        {
          const size_t data_size = sizeof(float) * in_out.count * 3;
          float *cpy_data = (float*)malloc(data_size);
          memcpy(cpy_data, in_out.position_vec3, data_size);
          
          cpy_in_out.position_vec3 = cpy_data;
        }
        
        if(in_out.normal_vec3)
        {
          const size_t data_size = sizeof(float) * in_out.count * 3;
          float *cpy_data = (float*)malloc(data_size);
          memcpy(cpy_data, in_out.normal_vec3, data_size);
          
          cpy_in_out.normal_vec3 = cpy_data;
        }
        
        if(in_out.texture_coords_vec2)
        {
          const size_t data_size = sizeof(float) * in_out.count * 2;
          float *cpy_data = (float*)malloc(data_size);
          memcpy(cpy_data, in_out.texture_coords_vec2, data_size);
          
          cpy_in_out.texture_coords_vec2 = cpy_data;
        };
        
        if(in_out.color_vec4)
        {
          const size_t data_size = sizeof(float) * in_out.count * 4;
          float *cpy_data = (float*)malloc(data_size);
          memcpy(cpy_data, in_out.color_vec4, data_size);
          
          cpy_in_out.color_vec4 = cpy_data;
        };
      }
    }
    
    const uint32_t new_id = keys.size();
    
    in_out.id = new_id;
    cpy_in_out.id = new_id;
    
    keys.emplace_back(key);
    meshes.emplace_back(cpy_in_out);
    
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
