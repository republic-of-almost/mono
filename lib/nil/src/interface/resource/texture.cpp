#include <nil/resource/texture.hpp>
#include <internal_data/resources/texture.hpp>
#include <lib/array.hpp>
#include <lib/key.hpp>
#include <lib/string_pool.hpp>
#include <lib/logging.hpp>
#include <lib/assert.hpp>
#include <lib/key.hpp>
#include <lib/string.hpp>


namespace {


// ------------------------------------------------------------- [ Resource ] --



Nil_texture_data&
get_tex_data()
{
  static Nil_texture_data tex_data;
  return tex_data;
};


// ------------------------------------------------------------- [ Messages ] --


constexpr char msg_texture_no_name[]     = "Loading a Texture - must have a name.";
constexpr char msg_texture_name_exists[] = "Texture with name %s already exists";
constexpr char msg_texture_failed[]      = "Failed to add Texture %s";

}

/* ------------------------------------------------- [ Resource Lifetime ] -- */


bool
nil_rsrc_texture_initialize(Nil_ctx *ctx)
{
  return true;
}


bool
nil_rsrc_texture_destroy(Nil_ctx *ctx)
{
  return true;
}


/* --------------------------------------------------- [ Resource Access ] -- */


bool
nil_rsrc_texture_find_by_name(Nil_ctx *ctx, const char *name, Nil_texture *out)
{
  const uint32_t find_key = lib::string_pool::find(name);
  
  for(size_t i = 0; i < get_tex_data().keys.size(); ++i)
  {
    if(get_tex_data().keys[i] == find_key)
    {
      if(out)
      {
        out = &get_tex_data().textures[i];
      }
      
      return true;
    }
  }
  
  return false;
}


bool
nil_rsrc_texture_find_by_id(Nil_ctx *ctx, uint32_t id, Nil_texture *out)
{
  const uint32_t *ids = get_tex_data().keys.data();
  const size_t id_count = get_tex_data().keys.size();
  size_t index = 0;
  
  if(lib::key::linear_search(id, ids, id_count))
  {
    if(out)
    {
      out = &get_tex_data().textures[index];
    }
    
    return true;
  }
  
  return false;
}


void
nil_rsrc_texture_get_data(Nil_ctx *ctx, size_t *out_count, Nil_texture **out_data)
{
  *out_count = get_tex_data().keys.size();
  *out_data = get_tex_data().textures.data();
}


bool
nil_rsrc_texture_get_by_id(Nil_ctx *ctx, uint32_t id, Nil_texture **out)
{
  const size_t count = get_tex_data().textures.size();
  
  if(count < id)
  {
    *out = &get_tex_data().textures[id];

    return true;
  }
  
  return false;
}


/* -------------------------------------------------- [ Resource Details ] -- */


size_t
nil_rsrc_texture_get_count(Nil_ctx *ctx)
{
  return get_tex_data().keys.size();
}


/* ---------------------------------------------------- [ Resource Batch ] -- */


void
nil_rsrc_texture_create_batch(Nil_ctx *ctx, Nil_texture *in, size_t count, bool move)
{
}


/* ------------------------------------------------- [ Resource Instance ] -- */


uint32_t
nil_rsrc_texture_create(Nil_ctx *ctx, Nil_texture *in_out, bool move)
{
  // -- Param Check -- //
  {
    const bool has_name   = !!in_out->name;
    const bool has_length = strlen(in_out->name);

    LIB_ASSERT(has_name);
    LIB_ASSERT(has_length);

    if (!has_name || !has_length)
    {
      LOG_ERROR(msg_texture_no_name);

      return false;
    }
  }

  // -- Check and return if exists, no support for updating atm -- //
  {
    const uint32_t check_key = lib::string_pool::find(in_out->name);

    if(check_key)
    {
      LOG_WARNING(msg_texture_name_exists, in_out->name);
      return false;
    }
  }

  // -- Load new Texture -- //
  Nil_texture cpy = *in_out;
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
        const size_t cpy_name_size = sizeof(decltype(*in_out->name)) * (strlen(in_out->name) + 1);
        cpy_name = (char*)malloc(cpy_name_size);

        if(cpy_name)
        {
          memset(cpy_name, 0, cpy_name_size);
          memcpy(cpy_name, in_out->name, cpy_name_size);
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
        const size_t data_size = in_out->data_size;
        cpy_data = (uintptr_t)malloc(data_size);

        if(cpy_data)
        {
          memset((void*)cpy_data, 0, data_size);
          memcpy((void*)cpy_data, (void*)in_out->data, data_size);
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

        LOG_ERROR(msg_texture_failed, in_out->name);

        LIB_ASSERT(false);
        return false;
      }
    }

    // Set Outputs //
    {
      // Generate new id //
      {
        const uint32_t new_id = get_tex_data().keys.size();
        in_out->id = new_id;
        cpy.id = in_out->id;
      }

      // Normalize other outputs //
      {
        if(in_out->data_type != NIL_DATA_LOCAL)
        {
          in_out->status = NIL_RSRC_STATUS_NONE;
          in_out->platform_resource = 0;
        }
        cpy.status = in_out->status;

        cpy.platform_resource = in_out->platform_resource;
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


bool
nil_rsrc_texture_destroy(Nil_ctx *ctx, uint32_t id)
{
  return false; // not supported
}


bool
nil_rsrc_texture_load(Nil_ctx *ctx, uint32_t id)
{
  return false;
}
