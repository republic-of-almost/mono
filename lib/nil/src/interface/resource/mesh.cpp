#include <nil/resource/mesh.hpp>
#include <internal_data/internal_data.hpp>
#include <internal_data/resources/mesh.hpp>
#include <lib/array.hpp>
#include <lib/string_pool.hpp>
#include <lib/string.hpp>
#include <lib/logging.hpp>
#include <lib/assert.hpp>
#include <lib/key.hpp>
#include <math/geometry/aabb.hpp>
#include <common/common.hpp>


namespace {


// ------------------------------------------------------------- [ Messages ] --


constexpr char msg_mesh_no_name[]       = "Loading a Mesh - must have a name";
constexpr char msg_mesh_has_no_vertex[] = "Mesh %s has no vertex data";
constexpr char msg_mesh_name_exists[]   = "Mesh with name %s already exists";
constexpr char msg_mesh_failed[]        = "Failed to add Mesh %s";


// ---------------------------------------------------------- [ Identifiers ] --


constexpr char mesh_type_name[] = "Mesh";


} // anon ns



/* ------------------------------------------------- [ Resource Lifetime ] -- */


bool
nil_rsrc_mesh_initialize(Nil_ctx *ctx)
{
  return true;
}


bool
nil_rsrc_mesh_destroy(Nil_ctx *ctx)
{
  return true;
}


/* --------------------------------------------------- [ Resource Access ] -- */


bool
nil_rsrc_mesh_find_by_name(Nil_ctx *ctx, const char *name, Nil_mesh *out)
{
  const size_t count = ctx->rsrc_mesh->keys.size();
  const Nil_mesh *data = ctx->rsrc_mesh->meshes.data();
  
  for(size_t i = 0; i < count; ++i)
  {
    if(data[i].name && strcmp(data[i].name, name) == 0)
    {
      if(out)
      {
        out = &ctx->rsrc_mesh->meshes[i];
      }
      
      return true;
    }
  }
  
  return false;
}


bool
nil_rsrc_mesh_find_by_id(Nil_ctx *ctx, uint32_t id, Nil_mesh *out)
{
  const uint32_t *ids = ctx->rsrc_mesh->keys.data();
  const size_t id_count = ctx->rsrc_mesh->keys.size();
  size_t index = 0;
  
  if(lib::key::linear_search(id, ids, id_count))
  {
    if(out)
    {
      out = &ctx->rsrc_mesh->meshes[index];
    }
    
    return true;
  }
  
  return false;
}


void
nil_rsrc_mesh_get_data(Nil_ctx *ctx, size_t *out_count, Nil_mesh **out_data)
{
  *out_count = ctx->rsrc_mesh->keys.size();
  *out_data = ctx->rsrc_mesh->meshes.data();
}


bool
nil_rsrc_texture_get_by_id(Nil_ctx *ctx, uint32_t id, Nil_mesh **out)
{
  const size_t count = ctx->rsrc_mesh->meshes.size();
  
  if(count > id)
  {
    *out = &ctx->rsrc_mesh->meshes[id];

    return true;
  }
  
  return false;
}


/* -------------------------------------------------- [ Resource Details ] -- */


size_t
nil_rsrc_mesh_get_count(Nil_ctx *ctx)
{
  return ctx->rsrc_mesh->keys.size();
}


/* ---------------------------------------------------- [ Resource Batch ] -- */


void
nil_rsrc_mesh_create_batch(Nil_ctx *ctx, Nil_mesh *in, size_t count, bool move)
{
}


/* ------------------------------------------------- [ Resource Instance ] -- */


uint32_t
nil_rsrc_mesh_create(Nil_ctx *ctx, Nil_mesh *in_out, bool move)
{
  // -- Param Check -- //
  {
    // Must have valid name
    {
      const bool has_name   = !!in_out->name;
      const bool has_length = strlen(in_out->name);

      LIB_ASSERT(has_name);
      LIB_ASSERT(has_length);

      if (!has_name || !has_length)
      {
        LOG_ERROR(msg_mesh_no_name);

        return 0;
      }
    }

    // Must have valid mesh data
    {
      const bool has_position = !!in_out->position_vec3;
      const bool has_color    = !!in_out->color_vec4;
      const bool has_normal   = !!in_out->normal_vec3;
      const bool has_texc     = !!in_out->texture_coords_vec2;
      
      const bool has_something = has_position | has_color | has_normal | has_texc;
      const bool has_verts     = !!in_out->triangle_count;
      
      LIB_ASSERT(has_something);
      LIB_ASSERT(has_verts);
      
      if (!has_something || !has_verts)
      {
        LOG_ERROR(msg_mesh_has_no_vertex, in_out->name);
        return 0;
      }
    }
  }
  
  // -- Check and return if exists, no support for updating atm -- //
  {
    const uint32_t check_key = lib::string_pool::find(in_out->name);

    if(check_key)
    {
      size_t index = 0;
      if (lib::key::linear_search(check_key, ctx->rsrc_mesh->keys.data(), ctx->rsrc_mesh->keys.size(), &index))
      {
        LOG_WARNING(msg_mesh_name_exists, in_out->name);
        return 0;
      }
    }
  }
  
  // -- Load new Mesh -- //
  Nil_mesh cpy = *in_out;
  {
    // Transfer ownership //
    /*
      Do this first incase it fails we can cleanup.
    */
    {
      bool failed = false;
 
      // Copy the name //
      char *cpy_name = nullptr;

      if(!failed)
      {
        failed = !Nil_detail::copy_data_name(&cpy_name, in_out->name, malloc);
      }

      // Copy all the mesh data //
      float *pos_data   = nullptr;
      float *norm_data  = nullptr;
      float *texc_data  = nullptr;
      float *color_data = nullptr;
      uint32_t *index_data = nullptr;

      if(!failed)
      {
        if (in_out->position_vec3 && !failed)
        {
          const size_t data_size = sizeof(float) * in_out->triangle_count * 3;
          failed = !Nil_detail::copy_data((void**)&pos_data, (void*)in_out->position_vec3, data_size, malloc);
        }

        if (in_out->normal_vec3 && !failed)
        {
          const size_t data_size = sizeof(float) * in_out->triangle_count * 3;
          failed = !Nil_detail::copy_data((void**)&norm_data, (void*)in_out->normal_vec3, data_size, malloc);
        }

        if (in_out->texture_coords_vec2 && !failed)
        {
          const size_t data_size = sizeof(float) * in_out->triangle_count * 2;
          failed = !Nil_detail::copy_data((void**)&texc_data, (void*)in_out->texture_coords_vec2, data_size, malloc);
        };

        if (in_out->color_vec4 && !failed)
        {
          const size_t data_size = sizeof(float) * in_out->triangle_count * 2;
          failed = !Nil_detail::copy_data((void**)&color_data, (void*)in_out->color_vec4, data_size, malloc);
        }
        
        if (in_out->index && !failed)
        {
          const size_t data_size = sizeof(uint32_t) * in_out->index_count;
          failed = !Nil_detail::copy_data((void**)&index_data, (void*)in_out->index, data_size, malloc);
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

        LOG_ERROR(msg_mesh_failed, in_out->name);
        LIB_ASSERT(false);
        
        return 0;
      }
    }

    // Set Outputs //
    {
      // Generate bounding box //
      {
        const math::aabb box(
          math::aabb_init(in_out->position_vec3, in_out->triangle_count * 3)
        );
      
        memcpy(
          in_out->bounding_box.min,
          box.min.data,
          sizeof(in_out->bounding_box.min)
        );
      
        memcpy(
          in_out->bounding_box.max,
          box.max.data,
          sizeof(in_out->bounding_box.max)
        );

        cpy.bounding_box = in_out->bounding_box;
      }
    
      // Generate new id //
      {
        const uint32_t new_id = ctx->rsrc_mesh->keys.size();
        in_out->id = new_id;
        cpy.id = in_out->id;
      }

      // Normalize other outputs //
      {
        in_out->status = NIL_RSRC_STATUS_PENDING;
        cpy.status = in_out->status;

        in_out->platform_resource = 0;
        cpy.platform_resource = in_out->platform_resource;
      }
    }
    
    // -- Save new Mesh Copy -- //
    {
      const uint32_t key = lib::string_pool::add(cpy.name);
      ctx->rsrc_mesh->keys.emplace_back(key);
      ctx->rsrc_mesh->meshes.emplace_back(cpy);
    }    
  }

  return in_out->id;
}


bool
nil_rsrc_mesh_destroy(Nil_ctx *ctx, uint32_t id)
{
  return false;
}


/* status */


bool
nil_rsrc_mesh_set_load_status(Nil_ctx *ctx, uint32_t id, Nil_resource_status new_status)
{
  Nil_texture *self = nullptr;
  
  const bool found  = nil_rsrc_texture_get_by_id(ctx, id, &self);
  
  if(found)
  {
    self->status = new_status;
    return true;
  }

  LOG_ERROR("Cant find or update Mesh.");
  return false;
}


Nil_resource_status
nil_rsrc_mesh_get_load_status(Nil_ctx *ctx, uint32_t id)
{
  Nil_texture *self;
  
  const bool found = nil_rsrc_texture_get_by_id(ctx, id, &self);
  
  if(found)
  {
    return self->status;
  }

  LOG_ERROR("Cant find Mesh.");
  return NIL_RSRC_STATUS_NONE;
}
