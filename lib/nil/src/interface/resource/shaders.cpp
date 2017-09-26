#include <nil/resource/shader.hpp>
#include <lib/array.hpp>
#include <lib/string_pool.hpp>
#include <lib/logging.hpp>
#include <lib/assert.hpp>
#include <lib/key.hpp>
#include <lib/string.hpp>
#include <lib/helpers.hpp>
#include <common/common.hpp>


namespace {


// ------------------------------------------------------------- [ Resource ] --


struct Shader_data
{
  lib::array<uint32_t, 128> keys{uint32_t{0}};
  lib::array<Nil::Resource::Shader, 128> shader{Nil::Resource::Shader{}};
};


Shader_data&
get_shd_data()
{
  static Shader_data shd_data;
  return shd_data;
};


// ------------------------------------------------------------- [ Messages ] --


constexpr char msg_shader_no_name[]     = "Sahder needs to have a name.";
constexpr char msg_shader_name_exists[] = "Shader with name %s already exists";
constexpr char msg_shader_failed[]      = "Failed to add Shader %s";


// ---------------------------------------------------------- [ Identifiers ] --


constexpr char shader_type_name[] = "Shader";
	

} // anon ns


namespace Nil {
namespace Resource {


// ----------------------------------------------------------------- [ Find ] --


bool
find_by_name(const char *name, Shader *out)
{
  // -- Param Check -- //
  {
    const bool has_name   = !!name;
    const bool has_length = strlen(name);

    LIB_ASSERT(has_name);
    LIB_ASSERT(has_length);

    if (!has_name || !has_length)
    {
      LOG_ERROR(msg_shader_no_name);

      return false;
    }
  }
  
  size_t count = get_shd_data().keys.size();
  Shader *shaders = get_shd_data().shader.data();
  
  for(size_t i = 0; i < count; ++i)
  {
    if(shaders[i].name)
    {
      if(strcmp(shaders[i].name, name) == 0)
      {
        if(out)
        {
          *out = shaders[i];
        }
        return true;
      }
    }
  }
  
  return false;
}


bool
find_by_index(const uint32_t index, Shader *out)
{
  size_t count = get_shd_data().keys.size();
  Shader *shaders = get_shd_data().shader.data();

  if(index < count)
  {
    *out = shaders[index];
    
    return true;
  }
  
  return false;
}


// ----------------------------------------------------------- [ Get / Load ] --


bool
load(Shader &in_out)
{
  // -- Existing Shader -- //
  Shader local{};
  {
    if(in_out.name && in_out.id)
    {
      find_by_index(in_out.id, &local);
    }
  }
  
  // -- Only process new or not submitted for load shaders -- //
  if(local.status != Nil::Resource::Load_status::NONE)
  {
    LOG_WARNING("Can't update shader after its loaded.");
    return false;
  }
  
  // -- Remove any heap data that has been updated. -- //
  {
    if(local.name != in_out.name)       { free((void*)local.name);    }
    if(local.vs_code != in_out.vs_code) { free((void*)local.vs_code); }
    if(local.gs_code != in_out.gs_code) { free((void*)local.gs_code); }
    if(local.fs_code != in_out.fs_code) { free((void*)local.fs_code); }
  }
  
  // -- Save Shader -- //
  {
    // Transfer ownership //
    {
      bool failed = false;

      // Copy the name //
      char *cpy_name = nullptr;

      if(!failed)
      {
        failed = !Nil_detail::copy_data_name(&cpy_name, in_out.name, malloc);
      }

      // Copy the shader code //
      char *cpy_shd_data[3] {};
      const char *curr_shd_data[] {in_out.vs_code, in_out.gs_code, in_out.fs_code};

      LIB_ASSERT(LIB_ARRAY_SIZE(cpy_shd_data) == LIB_ARRAY_SIZE(curr_shd_data));

      const size_t shd_cpy_size = LIB_ARRAY_SIZE(curr_shd_data);
      
      for(uint32_t i = 0; i < shd_cpy_size; ++i)
      {
        const bool has_data = curr_shd_data[i];
      
        if(!failed && has_data)
        {
          failed = !Nil_detail::copy_data_name(&cpy_shd_data[i], curr_shd_data[i], malloc);
        }
      }
   
      // If not failed add the new data, else delete and return.
      if(!failed)
      {
        local.name    = cpy_name;
        local.vs_code = cpy_shd_data[0];
        local.gs_code = cpy_shd_data[1];
        local.fs_code = cpy_shd_data[2];
      }
      else
      {
        if(cpy_name)        { free(cpy_name);        }
        if(cpy_shd_data[0]) { free(cpy_shd_data[0]); }
        if(cpy_shd_data[1]) { free(cpy_shd_data[1]); }
        if(cpy_shd_data[2]) { free(cpy_shd_data[2]); }

        LOG_ERROR(msg_shader_failed, in_out.name);
        LIB_ASSERT(false);
        
        return false;
      }
    }
    
    // Set Outputs //
    if(local.id == 0)
    {
      // Generate new id //
      {
        const uint32_t new_id = get_shd_data().keys.size();
        in_out.id = new_id;
        local.id = in_out.id;
      }

      // Normalize other outputs //
      {
        in_out.status = Load_status::NONE;
        in_out.platform_resource = 0;
        
        local.status = in_out.status;
        local.platform_resource = in_out.platform_resource;
      }
      
      // -- Save new Shader Copy -- //
      {
        const uint32_t key = in_out.id;
        get_shd_data().keys.emplace_back(key);
        get_shd_data().shader.emplace_back(local);
      }
    }
    else
    {
      // Update existing
      {
        get_shd_data().shader[local.id] = local;
      }
    }
  }

 return true;
}


void
get(size_t *count, Shader **in_out)
{
  *count = get_shd_data().keys.size();
  *in_out = get_shd_data().shader.data();
}


// ----------------------------------------------------------------- [ Info ] --


const char *
get_type_name(const Shader &)
{
  return shader_type_name;
}


size_t
shader_count()
{
  return get_shd_data().keys.size();
}


} // ns
} // ns
