#include <nil/resource/texture.hpp>
#include <lib/array.hpp>
#include <lib/string_pool.hpp>
#include <lib/logging.hpp>
#include <lib/assert.hpp>
#include <lib/key.hpp>
#include <lib/string.hpp>


// ------------------------------------------------------------- [ Resource ] --


namespace {


struct Texture_data {
  lib::array<uint32_t, 128> keys{uint32_t{0}};
  lib::array<Nil::Resource::Texture, 128> textures{Nil::Resource::Texture{}};
};


Texture_data&
get_tex_data()
{
  static Texture_data tex_data;
  return tex_data;
};


} // anon ns


namespace Nil {
namespace Resource {


// ----------------------------------------------------------------- [ Find ] --


bool
find_by_name(const char *name, Texture &out)
{
  const uint32_t find_key = lib::string_pool::find(name);
  
  for(size_t i = 0; i < get_tex_data().keys.size(); ++i)
  {
    if(get_tex_data().keys[i] == find_key)
    {
      out = get_tex_data().textures[i];
      return true;
    }
  }
  
  return false;
}


// ----------------------------------------------------------- [ Get / Load ] --


bool
load(Texture &in_out)
{
  // -- Param Check -- //
  {
    const bool has_name   = !!in_out.name;
    const bool has_length = strlen(in_out.name);

    LIB_ASSERT(has_name);
    LIB_ASSERT(has_length);

    if (!has_name || !has_length)
    {
      char msg[2048]{};
      sprintf(msg, "Loading a Texture - must have a name.", in_out.name);

      LOG_ERROR(msg);

      return false;
    }
  }

  // -- Check and return if exists, no support for updating atm -- //
  const uint32_t check_key = lib::string_pool::find(in_out.name);

  if(check_key)
  {
    char msg[2048]{};
    sprintf(msg, "Texture with name %s already exists", in_out.name);

    LOG_WARNING(msg);
    LIB_ASSERT(false);
    return false;
  }

  // -- Load new Texture -- //
  Texture cpy = in_out;
  {
    // Transfer ownership //
    /*
      Do this first because if it fails we need to unset output data.
    */
    {
      bool failed = false;

      // Copy the name //
      char *cpy_name = nullptr;

      if(!failed)
      {
        const size_t cpy_name_size = sizeof(decltype(*in_out.name)) * (strlen(in_out.name) + 1);
        cpy_name = (char*)malloc(cpy_name_size);

        if(cpy_name)
        {
          memset(cpy_name, 0, cpy_name_size);
          memcpy(cpy_name, in_out.name, cpy_name_size);
        }
        else
        {
          failed = true;
        }
      }

      // Copy the data //
       uintptr_t cpy_data;

      if(!failed)
      {
        const size_t data_size = in_out.data_size;
        cpy_data = (uintptr_t)malloc(data_size);

        if(cpy_data)
        {
          memset((void*)cpy_data, 0, data_size);
          memcpy((void*)cpy_data, (void*)in_out.data, data_size);
        }
      }

      // If not failed add the new data, else delete and return.
      if(!failed)
      {
        cpy.data = cpy_data;
        cpy.name = cpy_name;
      }
      else
      {
        if(cpy_data) { free((void*)cpy_data); }
        if(cpy_name) { free(cpy_name);        }

        char msg[2048]{};
        sprintf(msg, "Failed to add Texture %s", in_out.name);

        LOG_ERROR(msg);

        LIB_ASSERT(false);
        return false;
      }
    }

    // Set Outputs //
    {
      // Generate new id //
      {
        const uint32_t new_id = get_tex_data().keys.size();
        in_out.id = new_id;
        cpy.id = in_out.id;
      }

      // Normalize other outputs //
      {
        in_out.status = Texture::PENDING;
        cpy.status = in_out.status;

        in_out.platform_resource = 0;
        cpy.platform_resource = in_out.platform_resource;
      }
    }
  }

  // -- Save new Texture Copy -- //
  {
    const uint32_t key = lib::string_pool::add(cpy.name);
    get_tex_data().keys.emplace_back(key);
    get_tex_data().textures.emplace_back(cpy);
  }

   return true;
}


void
get(size_t *count, Texture **in_out)
{
  *count = get_tex_data().keys.size();
  *in_out = get_tex_data().textures.data();
}


// ----------------------------------------------------------------- [ Info ] --


const char *
get_type_name(const Texture &)
{
  return "Texture";
}



} // ns
} // ns
