#include <nil/resource/material.hpp>
#include <internal_data/internal_data.hpp>
#include <internal_data/resources/material_data.hpp>
#include <lib/array.hpp>
#include <lib/key.hpp>
#include <lib/string_pool.hpp>
#include <lib/logging.hpp>
#include <lib/string.hpp>
#include <lib/assert.hpp>


/* ------------------------------------------------- [ Resource Lifetime ] -- */


bool
nil_rsrc_material_initialize(Nil_ctx *ctx)
{
  return true;
}


bool
nil_rsrc_material_destroy(Nil_ctx *ctx)
{
  return true;
}


/* --------------------------------------------------- [ Resource Access ] -- */


bool
nil_rsrc_material_find_by_name(Nil_ctx *ctx, const char *name, Nil_material *out)
{
  // -- Param Check -- //
  {
    const bool has_name   = !!name;
    const bool has_length = strlen(name);

    LIB_ASSERT(has_name);
    LIB_ASSERT(has_length);

    if (!has_name || !has_length)
    {
      LIB_ASSERT(false);

      return false;
    }
  }
  
  size_t count       = ctx->rsrc_material->keys.size();
  Nil_material *mats = ctx->rsrc_material->materials.data();
  
  for(size_t i = 0; i < count; ++i)
  {
    if(mats[i].name)
    {
      if(strcmp(mats[i].name, name) == 0)
      {
        if(out)
        {
          *out = mats[i];
        }
        return true;
      }
    }
  }
  
  return false;
}


bool
nil_rsrc_material_find_by_id(Nil_ctx *ctx, uint32_t id, Nil_material *out)
{
  size_t count = ctx->rsrc_material->keys.size();
  Nil_material *mats = ctx->rsrc_material->materials.data();

  if(id < count)
  {
    *out = mats[id];
    
    return true;
  }
  
  return false;
}


void
nil_rsrc_material_get_data(Nil_ctx *ctx, size_t *out_count, Nil_material **out_data)
{
  *out_count = ctx->rsrc_material->keys.size();
  *out_data = ctx->rsrc_material->materials.data();
}


bool
nil_rsrc_material_get_by_id(Nil_ctx *ctx, uint32_t id, Nil_material **out)
{
  size_t count = 0;
  Nil_material *data = nullptr;
  
  nil_rsrc_material_get_data(ctx, &count, &data);
  
  if(id < count)
  {
    *out = &data[id];
    return true;
  }
  
  LOG_WARNING("Can't find mat");
  return false;
}


/* -------------------------------------------------- [ Resource Details ] -- */


size_t
nil_rsrc_material_get_count(Nil_ctx *ctx)
{
  return ctx->rsrc_material->keys.size();
}


/* ---------------------------------------------------- [ Resource Batch ] -- */


void
nil_rsrc_material_create_batch(Nil_ctx *ctx, Nil_material *in_out, size_t count, bool move)
{
}


/* ------------------------------------------------- [ Resource Instance ] -- */


uint32_t
nil_rsrc_material_create(Nil_ctx *ctx, Nil_material *in_out, bool move)
{
  // -- Param Check -- //
  {
    const bool has_name   = !!in_out->name;
    const bool has_length = strlen(in_out->name);

    if (!has_name || !has_length)
    {
      return false;
    }
  }

  // -- Update Existing Material -- //
  const uint32_t key = lib::string_pool::find(in_out->name);

  if (key)
  {
    size_t index = 0;
    if (lib::key::linear_search(key, ctx->rsrc_material->keys.data(), ctx->rsrc_material->keys.size(), &index))
    {
      // Set outputs //
      in_out->id = ctx->rsrc_material->materials[index].id;

      // Update local changes //
      Nil_material *local_mat = &(ctx->rsrc_material->materials[index]);
      const char *local_name  = local_mat->name; // Transfer local copy
      *local_mat              = *in_out;
      local_mat->name         = local_name;
      
      return true;
    }
  }
  
  // -- Load new Material -- //
  Nil_material cpy = *in_out;
  {
    // Transfer ownership //
    /*
      Do this first because if it fails we would need to unset output data.
    */
    {
      const size_t cpy_name_size = sizeof(decltype(*in_out->name)) * (strlen(in_out->name) + 1);
      char *cpy_name = (char*)malloc(cpy_name_size);

      if (!cpy_name)
      {
        LIB_ASSERT(false);
        
        return false;
      }

      memset(cpy_name, 0, cpy_name_size);
      memcpy(cpy_name, in_out->name, cpy_name_size);

      cpy.name = cpy_name;
    }

    // Set Outputs //
    {
      in_out->id = ctx->rsrc_material->keys.size();
      cpy.id = in_out->id;
    }
  }

  // -- Save new Material Copy -- //
  {
    const uint32_t new_key = lib::string_pool::add(cpy.name);

    ctx->rsrc_material->keys.emplace_back(new_key);
    ctx->rsrc_material->materials.emplace_back(cpy);
  }
  
  return true;
}


bool
nil_rsrc_material_destroy(Nil_ctx *ctx, uint32_t id)
{
  LIB_ASSERT(false);
}


/* status */


bool
nil_rsrc_material_set_load_status(Nil_ctx *ctx, uint32_t id, Nil_resource_status status)
{
}


Nil_resource_status
nil_rsrc_material_get_load_status(Nil_ctx *ctx, uint32_t id)
{
  Nil_material *self;
  
  const bool found = nil_rsrc_material_get_by_id(ctx, id, &self);
  
  if(found)
  {
    return self->status;
  }

  LOG_ERROR("Cant find shader.");
  return NIL_RSRC_STATUS_NONE;
}

