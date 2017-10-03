#include <nil/resource/audio.hpp>
#include <internal_data/resources/audio_src.hpp>
#include <internal_data/internal_data.hpp>
#include <lib/array.hpp>
#include <lib/string_pool.hpp>
#include <lib/logging.hpp>
#include <lib/assert.hpp>
#include <lib/key.hpp>
#include <lib/string.hpp>
#include <common/common.hpp>


namespace {


	
// ------------------------------------------------------------- [ Messages ] --


constexpr char msg_audio_no_name[]      = "Loading an Audio file - must have a name";
constexpr char msg_audio_no_data[]      = "Audio file %s has no data";
constexpr char msg_audio_name_exists[]  = "Audio file with name %s already exists";
constexpr char msg_audio_failed[]       = "Failed to add Audio file %s";


// ---------------------------------------------------------- [ Identifiers ] --


constexpr char audio_type_name[] = "Audio";


} // anon ns




/* ------------------------------------------------- [ Resource Lifetime ] -- */


bool
nil_rsrc_audio_src_initialize(Nil_ctx *ctx)
{
  return true;
}


bool
nil_rsrc_audio_src_destroy(Nil_ctx *ctx)
{
  return true;
}


/* --------------------------------------------------- [ Resource Access ] -- */


bool
nil_rsrc_audio_src_find_by_name(Nil_ctx *ctx, const char *name, Nil_audio_src *out)
{
  const size_t count = ctx->rsrc_audio->keys.size();
  const Nil_audio_src *data = ctx->rsrc_audio->audio.data();
  
  for(size_t i = 0; i < count; ++i)
  {
    if(data[i].name && strcmp(data[i].name, name) == 0)
    {
      if(out)
      {
        *out = ctx->rsrc_audio->audio[i];
      }
      
      return true;
    }
  }
  
  return false;
}


bool
nil_rsrc_audio_src_find_by_id(Nil_ctx *ctx, uint32_t id, Nil_audio_src *out)
{
  const uint32_t *ids = ctx->rsrc_audio->keys.data();
  const size_t id_count = ctx->rsrc_audio->keys.size();
  size_t index = 0;
  
  if(lib::key::linear_search(id, ids, id_count))
  {
    if(out)
    {
      out = &ctx->rsrc_audio->audio[index];
    }
    
    return true;
  }
  
  return false;
}


void
nil_rsrc_audio_src_get_data(Nil_ctx *ctx, size_t *out_count, Nil_audio_src **out_data)
{
  *out_count = ctx->rsrc_audio->keys.size();
  *out_data = ctx->rsrc_audio->audio.data();
}


bool
nil_rsrc_audio_src_get_by_id(Nil_ctx *ctx, uint32_t id, Nil_audio_src **out)
{
  const size_t count = ctx->rsrc_audio->audio.size();
  
  if(count > id)
  {
    *out = &ctx->rsrc_audio->audio[id];

    return true;
  }
  
  return false;
}


/* -------------------------------------------------- [ Resource Details ] -- */


size_t
nil_rsrc_audio_src_get_count(Nil_ctx *ctx)
{
  return ctx->rsrc_audio->keys.size();
}


/* ---------------------------------------------------- [ Resource Batch ] -- */


void
nil_rsrc_audio_src_create_batch(Nil_ctx *ctx, Nil_audio_src *in_out, size_t count, bool move)
{
  LIB_ASSERT(false);
}


/* ------------------------------------------------- [ Resource Instance ] -- */


uint32_t
nil_rsrc_audio_src_create(Nil_ctx *ctx, Nil_audio_src *in_out, bool move)
{
  // -- Param Check -- //
  {
    const bool has_name     = !!in_out->name;
    const bool has_length   = strlen(in_out->name);
    const bool has_data     = !!in_out->data;
    const bool has_data_len = !!in_out->data_size;
    
    LIB_ASSERT(has_name);
    LIB_ASSERT(has_length);
    LIB_ASSERT(has_data);
    
    if(!has_name || !has_length)
    {
      LOG_ERROR(msg_audio_no_name);
      return 0;
    }
    
    if(!has_data || !has_data_len)
    {
      LOG_ERROR(msg_audio_no_data, in_out->name);
      return 0;
    }
  }
  
  // -- Check if file is loaded -- //
  {
    const uint32_t check_key = lib::string_pool::find(in_out->name);
    

    if(check_key)
    {
      size_t index = 0;
    
      if (lib::key::linear_search(check_key, ctx->rsrc_audio->keys.data(), ctx->rsrc_audio->keys.size(), &index))
      {
        LOG_WARNING(msg_audio_name_exists, in_out->name);
        return false;
      }
    }
  }
  
  // -- Load Audio -- //
  Nil_audio_src cpy = *in_out;
  {
    // Transfer Ownership //
    /*
      Do this first incase it fails we can cleanup.
    */
    {
      bool failed = false;
      
      // Copy Name //
      char *cpy_name = nullptr;
      {
        if(!failed)
        {
          failed = !Nil_detail::copy_data_name(&cpy_name, in_out->name, malloc);
        }
      }
      
      // Copy Data //
      void *cpy_data = nullptr;
      {
        if(!failed)
        {
          failed = !Nil_detail::copy_data(&cpy_data, (void*)in_out->data, in_out->data_size, malloc);
        }
      }
    
      // Set Data //
      {
        if(!failed)
        {
          cpy.name = cpy_name;
          cpy.data = (uintptr_t)cpy_data;
        }
        else
        {
          if(cpy_data) { free((void*)cpy_data); }
          if(cpy_name) { free(cpy_name);        }
        
          LOG_ERROR(msg_audio_failed, in_out->name);
        
          LIB_ASSERT(false);
          return false;
        }
      }
    }
  }
  
  // Generate new id //
  {
    const uint32_t new_id = ctx->rsrc_audio->keys.size();
    in_out->id = new_id;
    cpy.id = in_out->id;
  }
  
  // -- Save new Mesh -- //
  {
    const uint32_t key = lib::string_pool::add(cpy.name);
    ctx->rsrc_audio->keys.emplace_back(key);
    ctx->rsrc_audio->audio.emplace_back(cpy);
  }
  
  return in_out->id;
}


bool
nil_rsrc_audio_src_destroy(Nil_ctx *ctx, uint32_t id)
{
  LIB_ASSERT(false);
  return false;
}


/* status */


bool
nil_rsrc_audio_src_set_load_status(Nil_ctx *ctx, uint32_t id, Nil_resource_status new_status)
{
  Nil_audio_src *self = nullptr;
  
  const bool found = nil_rsrc_audio_src_get_by_id(ctx, id, &self);
  
  if(found)
  {
    self->status = new_status;
    return true;
  }

  LOG_ERROR("Cant find or update Audio src.");
  return false;
}


Nil_resource_status
nil_rsrc_audio_src_get_load_status(Nil_ctx *ctx, uint32_t id)
{
  Nil_audio_src *self = nullptr;
  
  const bool found = nil_rsrc_audio_src_get_by_id(ctx, id, &self);
  
  if(found)
  {
    return self->status;
  }

  LOG_ERROR("Cant find Audio src.");
  return NIL_RSRC_STATUS_NONE;
}
