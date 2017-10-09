#include <nil/resource/texture.hpp>
#include <common/utilities.hpp>
#include <common/error_msgs.hpp>
#include <internal_data/internal_data.hpp>
#include <internal_data/resources/texture_data.hpp>
#include <lib/array.hpp>
#include <lib/key.hpp>
#include <lib/string_pool.hpp>
#include <lib/logging.hpp>
#include <lib/string.hpp>


/* ------------------------------------------------- [ Resource Lifetime ] -- */


bool
nil_rsrc_texture_initialize(Nil_ctx *ctx)
{
  LOG_TODO("CTX Data should be init here rather than in ctx init");
  return true;
}


bool
nil_rsrc_texture_destroy(Nil_ctx *ctx)
{
  LOG_TODO("CTX Data should be destroyed here.");
  return true;
}


/* --------------------------------------------------- [ Resource Access ] -- */


bool
nil_rsrc_texture_find_by_name(Nil_ctx *ctx, const char *name, Nil_texture *out)
{
  /* Param Check */
  #ifdef NIL_PEDANTIC
  {
    const bool has_ctx = !!ctx;
    const bool has_name = !!name;

    NIL_ASSERT(has_ctx);
    NIL_ASSERT(has_name);

    if(!has_ctx)
    {
      LOG_ERROR(nil_err_msg_param_fail_no_ctx);
      return false;
    }

    if(!has_name)
    {
      LOG_ERROR(nil_err_msg_rsrc_name_required, "Texture");
      return false;
    }
  }
  #endif
  
  /* Search by name */
  {
    const uint32_t find_key = lib::string_pool::find(name);
    const size_t count = ctx->rsrc_texture->keys.size();
    
    for(size_t i = 0; i < count; ++i)
    {
      if(ctx->rsrc_texture->keys[i] == find_key)
      {
        if(out)
        {
          out = &ctx->rsrc_texture->textures[i];
        }
        
        return true;
      }
    }
  }
  
  return false;
}


bool
nil_rsrc_texture_find_by_id(Nil_ctx *ctx, uint32_t id, Nil_texture *out)
{
  /* Param Check */
  #ifdef NIL_PEDANTIC
  {
    const bool has_ctx = !!ctx;
    const bool has_name = !!id;

    NIL_ASSERT(has_ctx);
    NIL_ASSERT(has_name);

    if(!has_ctx)
    {
      LOG_ERROR(nil_err_msg_param_fail_no_ctx);
      return false;
    }

    if(!has_name)
    {
      LOG_ERROR(nil_err_msg_rsrc_id_required, "Texture");
      return false;
    }
  }
  #endif
  
  /* Search by ID */
  {
    const uint32_t *ids = ctx->rsrc_texture->keys.data();
    const size_t id_count = ctx->rsrc_texture->keys.size();
    size_t index = 0;
    
    if(lib::key::linear_search(id, ids, id_count))
    {
      if(out)
      {
        out = &ctx->rsrc_texture->textures[index];
      }
      
      return true;
    }
  }
  
  return false;
}


void
nil_rsrc_texture_get_data(Nil_ctx *ctx, size_t *out_count, Nil_texture **out_data)
{
  /* Param Check */
  #ifdef NIL_PEDANTIC
  {
    const bool has_ctx = !!ctx;
    const bool has_count = !!out_count;
    const bool has_data = !!out_data;

    NIL_ASSERT(has_ctx);
    NIL_ASSERT(has_count);
    NIL_ASSERT(has_data);

    if(!has_ctx)
    {
      LOG_ERROR(nil_err_msg_param_fail_no_ctx);
      return;
    }

    if(!has_count)
    {
      LOG_ERROR(nil_err_msg_rsrc_no_count, "Texture");
      return;
    }
    
    if(!has_data)
    {
      LOG_ERROR(nil_err_msg_rsrc_no_data, "Texture");
      return;
    }
  }
  #endif

  /* fetch data */
  {
    if(out_count)
    {
      *out_count = ctx->rsrc_texture->keys.size();
    }
    
    if(out_data)
    {
      *out_data = ctx->rsrc_texture->textures.data();
    }
  }
}


bool
nil_rsrc_texture_get_by_id(Nil_ctx *ctx, uint32_t id, Nil_texture **out)
{
  /* param check */
  #ifdef NIL_PEDANTIC
  {
    const bool has_ctx = !!ctx;
    const bool has_id = !!id;
    
    LIB_ASSERT(has_ctx);
    LIB_ASSERT(has_id);
    
    if(!has_ctx)
    {
      LOG_ERROR(nil_err_msg_param_fail_no_ctx);
      return false;
    }
    
    if(!has_id)
    {
      LOG_ERROR(nil_err_msg_rsrc_id_required, "Texture");
      return false;
    }
  }
  #endif

  const size_t count = ctx->rsrc_texture->textures.size();
  
  if(count > id)
  {
    *out = &ctx->rsrc_texture->textures[id];

    return true;
  }
  
  return false;
}


/* -------------------------------------------------- [ Resource Details ] -- */


size_t
nil_rsrc_texture_get_count(Nil_ctx *ctx)
{
  /* param check */
  #ifdef NIL_PEDANTIC
  {
    const bool has_ctx = !!ctx;
    
    LIB_ASSERT(has_ctx);
    
    if(!has_ctx)
    {
      LOG_ERROR(nil_err_msg_param_fail_no_ctx);
      return 0;
    }
  }
  #endif

  return ctx->rsrc_texture->keys.size();
}


/* ---------------------------------------------------- [ Resource Batch ] -- */


/* batch functions removed for the time */


/* ------------------------------------------------- [ Resource Instance ] -- */


uint32_t
nil_rsrc_texture_create(Nil_ctx *ctx, Nil_texture *in_out)
{
  /* param check */
  #ifdef NIL_PEDANTIC
  {
    const bool has_ctx    = !!ctx;
    const bool has_name   = !!in_out->name;
    const bool has_length = strlen(in_out->name);

    LIB_ASSERT(has_ctx);
    LIB_ASSERT(has_name);
    LIB_ASSERT(has_length);

    if(!has_ctx)
    {
      LOG_ERROR(nil_err_msg_param_fail_no_ctx);
      return 0;
    }

    if (!has_name || !has_length)
    {
      LOG_ERROR(nil_err_msg_rsrc_name_required, "Texture");
      return 0;
    }
  }
  #endif

  /* Check and return if exists, no support for updating atm */
  {
    const uint32_t check_key = lib::string_pool::find(in_out->name);

    if(check_key)
    {
      LOG_WARNING(nil_err_msg_rsrc_exists, in_out->name);
      return 0;
    }
  }
  
  /* Load new Texture */
  Nil_texture cpy = *in_out;
  {
    /* Transfer ownership */
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

        LOG_ERROR(nil_err_msg_rsrc_failed, in_out->name, "Texture");

        LIB_ASSERT(false);
        return 0;
      }
    }

    // Set Outputs //
    {
      // Generate new id //
      {
        const uint32_t new_id = ctx->rsrc_texture->keys.size();
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
    ctx->rsrc_texture->keys.emplace_back(key);
    ctx->rsrc_texture->textures.emplace_back(cpy);
  }

  return in_out->id;
}


bool
nil_rsrc_texture_destroy(Nil_ctx *ctx, uint32_t id)
{
  return false; // not supported
}


bool
nil_rsrc_texture_set_load_status(Nil_ctx *ctx, uint32_t id, Nil_resource_status new_status)
{
  Nil_texture *self = nullptr;
  
  const bool found  = nil_rsrc_texture_get_by_id(ctx, id, &self);
  
  if(found)
  {
    self->status = new_status;
    return true;
  }

  LOG_ERROR("Cant find or update shader.");
  return false;
}


Nil_resource_status
nil_rsrc_texture_get_load_status(Nil_ctx *ctx, uint32_t id)
{
  Nil_texture *self;
  
  const bool found = nil_rsrc_texture_get_by_id(ctx, id, &self);
  
  if(found)
  {
    return self->status;
  }

  LOG_ERROR("Cant find shader.");
  return NIL_RSRC_STATUS_NONE;
}
