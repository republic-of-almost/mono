#include <nil/resource/mesh.hpp>
#include <lib/array.hpp>
#include <lib/string_pool.hpp>
#include <lib/string.hpp>
#include <lib/logging.hpp>
#include <lib/assert.hpp>
#include <lib/key.hpp>
#include <math/geometry/aabb.hpp>


namespace {


// ------------------------------------------------------------- [ Resource ] --


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


// ------------------------------------------------------------- [ Messages ] --


constexpr char msg_mesh_no_name[]       = "Loading a Mesh - must have a name";
constexpr char msg_mesh_has_no_vertex[] = "Mesh %s has no vertex data";
constexpr char msg_mesh_name_exists[]   = "Mesh with name %s already exists";
constexpr char msg_mesh_failed[]        = "Failed to add Mesh %s";


// ---------------------------------------------------------- [ Identifiers ] --


constexpr char mesh_type_name[] = "Mesh";


} // anon ns


namespace Nil {
namespace Resource {


// ----------------------------------------------------------------- [ Find ] --


bool
find_by_name(const char *name, Mesh &out)
{
  /*
    TODO: Key hashing needs sorting!
  */

  for(auto &m : get_mesh_data().meshes)
  {
    if(m.name)
    {
      if(strcmp(m.name, name) == 0)
      {
        out = m;
        return true;
      }
    }
  }
  
  return false;

//  const uint32_t find_key = lib::string_pool::find(name);
//  
//  for(size_t i = 0; i < get_mesh_data().keys.size(); ++i)
//  {
//    const uint32_t key = get_mesh_data().keys[i];
//  
//    if(key == find_key)
//    {
//      out = get_mesh_data().meshes[i];
//      return true;
//    }
//  }
//  
//  return false;
}


// ----------------------------------------------------------- [ Get / Load ] --


bool
load(Mesh &in_out)
{
  // -- Param Check -- //
  {
    // Must have valid name
    {
      const bool has_name   = !!in_out.name;
      const bool has_length = strlen(in_out.name);

      LIB_ASSERT(has_name);
      LIB_ASSERT(has_length);

      if (!has_name || !has_length)
      {
        LOG_ERROR(msg_mesh_no_name);

        return false;
      }
    }

    // Must have valid mesh data
    {
      const bool has_position = !!in_out.position_vec3;
      const bool has_color    = !!in_out.color_vec4;
      const bool has_normal   = !!in_out.normal_vec3;
      const bool has_texc     = !!in_out.texture_coords_vec2;
      
      const bool has_something = has_position | has_color | has_normal | has_texc;
      const bool has_verts     = !!in_out.triangle_count;
      
      LIB_ASSERT(has_something);
      LIB_ASSERT(has_verts);
      
      if (!has_something || !has_verts)
      {
        LOG_ERROR(msg_mesh_has_no_vertex, in_out.name);
        return false;
      }
    }
  }
  
  // -- Check and return if exists, no support for updating atm -- //
  const uint32_t check_key = lib::string_pool::find(in_out.name);

  if(check_key)
  {
    size_t index = 0;
    if (lib::key::linear_search(check_key, get_mesh_data().keys.data(), get_mesh_data().keys.size(), &index))
    {
      LOG_WARNING(msg_mesh_name_exists, in_out.name);
      return false;
    }
  }
  
  // -- Load new Mesh -- //
  Mesh cpy = in_out;
  {
    // Transfer ownership //
    /*
      Do this first because if it fails we would need to unset output data.
    */
    {
      bool failed = false;
 
      // Copy the name //
      char *cpy_name = nullptr;

      if(!failed)
      {
        const size_t cpy_name_size = sizeof(decltype(*in_out.name)) * (strlen(in_out.name) + 1);
        cpy_name = (char*)malloc(cpy_name_size);

        if (cpy_name)
        {
          memset(cpy_name, 0, cpy_name_size);
          memcpy(cpy_name, in_out.name, cpy_name_size);
        }
        else
        {
          failed = true;
        }
      }

      // Copy all the mesh data //
      float *pos_data   = nullptr;
      float *norm_data  = nullptr;
      float *texc_data  = nullptr;
      float *color_data = nullptr;
      uint32_t *index_data = nullptr;

      if(!failed)
      {
        if (in_out.position_vec3 && !failed)
        {
          const size_t data_size = sizeof(float) * in_out.triangle_count * 3;
          pos_data = (float*)malloc(data_size);
          
          if (pos_data)
          {
            memset(pos_data, 0, data_size);
            memcpy(pos_data, in_out.position_vec3, data_size);
          }
          else
          {
            failed = true;
          }
        }

        if (in_out.normal_vec3 && !failed)
        {
          const size_t data_size = sizeof(float) * in_out.triangle_count * 3;
          norm_data = (float*)malloc(data_size);

          if (norm_data)
          {
            memset(norm_data, 0, data_size);
            memcpy(norm_data, in_out.normal_vec3, data_size);
          }
          else
          {
            failed = true;
          }
        }

        if (in_out.texture_coords_vec2 && !failed)
        {
          const size_t data_size = sizeof(float) * in_out.triangle_count * 2;
          texc_data = (float*)malloc(data_size);

          if (texc_data)
          {
            memset(texc_data, 0, data_size);
            memcpy(texc_data, in_out.texture_coords_vec2, data_size);
          }
          else
          {
            failed = true;
          }
        };

        if (in_out.color_vec4 && !failed)
        {
          const size_t data_size = sizeof(float) * in_out.triangle_count * 4;
          color_data = (float*)malloc(data_size);

          if (color_data)
          {
            memset(color_data, 0, data_size);
            memcpy(color_data, in_out.color_vec4, data_size);
          }
          else
          {
            failed = true;
          }
        }
        
        if (in_out.index && !failed)
        {
          const size_t data_size = sizeof(uint32_t) * in_out.index_count;
          index_data = (uint32_t*)malloc(data_size);
          
          if (index_data)
          {
            memset(index_data, 0, data_size);
            memcpy(index_data, in_out.index, data_size);
          }
          else
          {
            failed = true;
          }
        }
      }

      // If not failed add the new data, else delete and return.
      if (!failed)
      {
        cpy.name                = cpy_name;
        cpy.position_vec3       = pos_data;
        cpy.normal_vec3         = norm_data;
        cpy.color_vec4          = color_data;
        cpy.texture_coords_vec2 = texc_data;
        cpy.index               = index_data;
      }
      else
      {
        if(cpy_name)    { free(cpy_name);   }
        if(pos_data)    { free(pos_data);   }
        if(norm_data)   { free(norm_data);  }
        if(color_data)  { free(color_data); }
        if(texc_data)   { free(texc_data);  }
        if(index_data)  { free(index_data); }

        LOG_ERROR(msg_mesh_failed, in_out.name);
        LIB_ASSERT(false);
        return false;
      }
    }

    // Set Outputs //
    {
      // Generate bounding box //
      {
        const math::aabb box(
          math::aabb_init(in_out.position_vec3, in_out.triangle_count * 3)
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

        cpy.bounding_box = in_out.bounding_box;
      }
    
      // Generate new id //
      {
        const uint32_t new_id = get_mesh_data().keys.size();
        in_out.id = new_id;
        cpy.id = in_out.id;
      }

      // Normalize other outputs //
      {
        in_out.status = Mesh::PENDING;
        cpy.status = in_out.status;

        in_out.platform_resource = 0;
        cpy.platform_resource = in_out.platform_resource;
      }
    }
    
    // -- Save new Mesh Copy -- //
    {
      const uint32_t key = lib::string_pool::add(cpy.name);
      get_mesh_data().keys.emplace_back(key);
      get_mesh_data().meshes.emplace_back(cpy);
    }    
  }

  return true;
}


void
get(size_t *count, Mesh **out)
{
  *count = get_mesh_data().keys.size();
  *out   = get_mesh_data().meshes.begin();
}


// ----------------------------------------------------------------- [ Info ] --


const char *
get_type_name(const Mesh &)
{
  return mesh_type_name;
}


size_t
mesh_count()
{
  return get_mesh_data().keys.size();
}


} // ns
} // ns
