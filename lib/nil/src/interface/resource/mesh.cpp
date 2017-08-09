#include <nil/resource/mesh.hpp>
#include <lib/array.hpp>
#include <lib/string_pool.hpp>
#include <lib/logging.hpp>
#include <math/geometry/aabb.hpp>
#include <stdio.h>


namespace {


struct Mesh_data {
  lib::array<uint32_t, 128> keys{uint32_t{0}};
  lib::array<Nil::Resource::Mesh, 128> meshes{Nil::Resource::Mesh{}};
};


Mesh_data&
get_mesh_data()
{
  static Mesh_data data;
  return data;
}


} // anon ns


namespace Nil {
namespace Resource {


// ----------------------------------------------------------------- [ Find ] --


void
find_by_name(const char *name, Mesh &out)
{
  const uint32_t find_key = lib::string_pool::find(name);
  
  for(size_t i = 0; i < get_mesh_data().keys.size(); ++i)
  {
    const uint32_t key = get_mesh_data().keys[i];
  
    if(key == find_key)
    {
      out = get_mesh_data().meshes[i];
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
    char msg[2048]{};
    strcat(msg, "Mesh with name '");
    strcat(msg, name);
    strcat(msg, "' already exists");
    LOG_WARNING(msg);
    return false;
  }
  else
  {
    // Generate bounding box
    {
      const math::aabb box(
        math::aabb_init(in_out.position_vec3, in_out.count * 3)
      );
      
      memcpy(
        in_out.bounding_box.min,
        box.min.data,
        sizeof(in_out.bounding_box.min)
      );
      
      memcpy(
        in_out.bounding_box.max,
        box.max.data,
        sizeof(in_out.bounding_box.max)
      );
    }
    
    // Generate new id
    {
      const uint32_t new_id = get_mesh_data().keys.size();
      in_out.id = new_id;
    }
  
    Mesh cpy_in_out = in_out;
    
//    if(in_out.status == 0)
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
    
    const uint32_t key = lib::string_pool::add(name);
    get_mesh_data().keys.emplace_back(key);
    get_mesh_data().meshes.emplace_back(cpy_in_out);
    
    return true;
  }
}


void
get(size_t *count, Mesh **out)
{
  *count = get_mesh_data().keys.size();
  *out = get_mesh_data().meshes.begin();
}


// ----------------------------------------------------------------- [ Info ] --


const char *
get_type_name(const Mesh &)
{
  return "Mesh";
}


size_t
mesh_count()
{
  return get_mesh_data().keys.size() - 1; // We have an inital mesh for failures.
}


} // ns
} // ns
