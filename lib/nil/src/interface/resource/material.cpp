#include <nil/resource/material.hpp>
#include <lib/array.hpp>
#include <lib/key.hpp>
#include <lib/string_pool.hpp>
#include <lib/logging.hpp>
#include <lib/string.hpp>
#include <lib/assert.hpp>


namespace {


// ------------------------------------------------------------- [ Resource ] --


struct Material_data
{
  lib::array<uint32_t, 128> keys{uint32_t{0}};
  lib::array<Nil::Resource::Material, 128> materials{Nil::Resource::Material{"", 0xFFFFFFFF}};
};


Material_data&
get_mat_data()
{
  static Material_data data;
  return data;
}


// ------------------------------------------------------------- [ Messages ] --


constexpr char msg_material_must_have_name[] = "Loading/Updating a Material - must have a name.";
constexpr char msg_material_failed_to_add[]  = "Failed to add Material %s";


// ---------------------------------------------------------- [ Identifiers ] --


constexpr char material_type_name[] = "Material";


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
load(Material &in_out)
{
  // -- Param Check -- //
  {
    const bool has_name   = !!in_out.name;
    const bool has_length = strlen(in_out.name);

    LIB_ASSERT(has_name);
    LIB_ASSERT(has_length);

    if (!has_name || !has_length)
    {
      LOG_ERROR(msg_material_must_have_name);
      return false;
    }
  }

  // -- Update Existing Material -- //
  const uint32_t key = lib::string_pool::find(in_out.name);

  if (key)
  {
    size_t index = 0;
    if (lib::key::linear_search(key, get_mat_data().keys.data(), get_mat_data().keys.size(), &index))
    {
      // Set outputs //
      in_out.id = get_mat_data().materials[index].id;

      // Update local changes //
      Material *local_mat     = &(get_mat_data().materials[index]);
      const char *local_name  = local_mat->name; // Transfer local copy
      *local_mat              = in_out;
      local_mat->name         = local_name;
      
      return true;
    }
  }
  
  // -- Load new Material -- //
  Material cpy = in_out;
  {
    // Transfer ownership //
    /*
      Do this first because if it fails we would need to unset output data.
    */
    {
      const size_t cpy_name_size = sizeof(decltype(*in_out.name)) * (strlen(in_out.name) + 1);
      char *cpy_name = (char*)malloc(cpy_name_size);

      if (!cpy_name)
      {
        LIB_ASSERT(false);
        LOG_ERROR(msg_material_failed_to_add, in_out.name);
        
        return false;
      }

      memset(cpy_name, 0, cpy_name_size);
      memcpy(cpy_name, in_out.name, cpy_name_size);

      cpy.name = cpy_name;
    }

    // Set Outputs //
    {
      in_out.id = get_mat_data().keys.size();
      cpy.id = in_out.id;
    }
  }

  // -- Save new Material Copy -- //
  {
    const uint32_t new_key = lib::string_pool::add(cpy.name);

    get_mat_data().keys.emplace_back(new_key);
    get_mat_data().materials.emplace_back(cpy);
  }
  
  return true;
}


void
get(size_t *count, Material **out)
{
  *count = get_mat_data().keys.size();
  *out   = get_mat_data().materials.data();
}


// ----------------------------------------------------------------- [ Info ] --


const char *
get_type_name(const Material &)
{
  return material_type_name;
}


} // ns
} // ns
