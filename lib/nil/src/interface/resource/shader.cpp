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
  lib::array<Nil_shader, 128> shader{Nil_shader{}};
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


/* ------------------------------------------------------------ [ Static ] -- */

/* rsrc lifetime */

bool
nil_rsrc_shader_initialize()
{
  get_shd_data(); // enough to init
  return true;
}


bool
nil_rsrc_shader_destroy()
{
  return true;
}


/* search / access */

bool
nil_rsrc_shader_find_by_name(const char *name, Nil_shader *out)
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
  
  size_t count        = get_shd_data().keys.size();
  Nil_shader *shaders = get_shd_data().shader.data();
  
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
nil_rsrc_shader_find_by_id(const uint32_t id, Nil_shader *out)
{
  size_t count = get_shd_data().keys.size();
  Nil_shader *shaders = get_shd_data().shader.data();

  if(id < count)
  {
    *out = shaders[id];
    
    return true;
  }
  
  return false;
}

void
nil_rsrc_shader_get_data(size_t *count, Nil_shader **data)
{
  *count = get_shd_data().keys.size();
  *data = get_shd_data().shader.data();
}


bool
nil_rsrc_shader_get_by_id(uint32_t id, Nil_shader **out)
{
  size_t count = 0;
  Nil_shader *data = nullptr;
  
  nil_rsrc_shader_get_data(&count, &data);
  
  if(id < count)
  {
    *out = &data[id];
    return true;
  }
  
  LOG_WARNING("Can't find shader");
  return false;
}


/* details */

size_t
nil_rsrc_shader_get_count()
{
  return get_shd_data().keys.size();
}


/* batch */

void
nil_rsrc_shader_create_batch(Nil_shader *in, size_t count, bool move)
{
  for(uint32_t i = 0; i < count; ++i)
  {
    Nil_shader *shd = &in[i];
  
    /* Requrement Check */
    {
      /* name exists */
      if(shd->name == NULL || strlen(shd->name) == 0)
      {
        shd->id = 0;
        continue;
      }
      
      /* name not in use */
      if(nil_rsrc_shader_find_by_name(shd->name))
      {
        shd->id = 0;
        continue;
      }
    }
    
    Nil_shader local = *shd;
    
    /* Transfer ownership */
    if(!move)
    {
      bool failed = false;

      /* Copy the name */
      char *cpy_name = nullptr;

      if(!failed)
      {
        failed = !Nil_detail::copy_data_name(&cpy_name, shd->name, malloc);
      }
      
      /* Copy the shader code */
      char *cpy_shd_data[3] {};
      const char *curr_shd_data[] {shd->vs_code, shd->gs_code, shd->fs_code};

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
   
      /* If not failed add the new data, else delete and continue. */
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

        LOG_ERROR(msg_shader_failed, shd->name);
        LIB_ASSERT(false);
        
        continue;
      }
    }
    else
    {
      local.name = shd->name;
      local.vs_code = shd->vs_code;
      local.gs_code = shd->gs_code;
      local.fs_code = shd->fs_code;
    }
    
    /* generate outputs */
    if(local.id == 0)
    {
      // Generate new id //
      {
        const uint32_t new_id = get_shd_data().keys.size();
        shd->id = new_id;
        local.id = shd->id;
      }

      /* Normalize other outputs */
      {
        shd->status = NIL_RSRC_STATUS_NONE;
        shd->platform_resource = 0;
        
        local.status = shd->status;
        local.platform_resource = shd->platform_resource;
      }
      
      /* save a new copy */
      {
        const uint32_t key = shd->id;
        get_shd_data().keys.emplace_back(key);
        get_shd_data().shader.emplace_back(local);
      }
    }
  }
}


/* ---------------------------------------------------------- [ Instance ] -- */


bool
nil_rsrc_copy_src_helper(const char **dest, const char *new_src)
{
  const size_t bytes = (strlen(new_src) + 1) * sizeof(char);
  const char *new_alloc = (char*)malloc(bytes);
  
  if(new_alloc)
  {
    free((void*)*dest);
    *dest = new_alloc;
    
    memcpy((void*)*dest, new_src, bytes);
  }
  
  return !!new_alloc;
}


/* lifetime */

uint32_t
nil_rsrc_shader_create(Nil_shader *shd, bool move)
{
  if(shd)
  {
    nil_rsrc_shader_create_batch(shd, 1, move);
    return shd->id;
  }
  else
  {
    Nil_shader shd;
    memset(&shd, 0, sizeof(shd));
    
    nil_rsrc_shader_create_batch(&shd, 1, false);
    return shd.id;
  }
}

bool
nil_rsrc_shader_destroy(uint32_t id)
{
  LOG_WARNING("Shader destruction not supported yet.");
  return false;
}


/* type */

bool
nil_rsrc_shader_set_type(uint32_t id, Nil_shader_type type)
{
  Nil_shader *self = nullptr;
  
  const bool found  = nil_rsrc_shader_get_by_id(id, &self);
  const bool status = (found && self->status == NIL_RSRC_STATUS_NONE);
  
  if(found && status)
  {
    self->type = type;
    return true;
  }

  LOG_ERROR("Cant find or update shader.");
  return false;
}

Nil_shader_type
nil_rsrc_shader_get_type(uint32_t id)
{
  Nil_shader *self;
  
  const bool found = nil_rsrc_shader_get_by_id(id, &self);
  
  if(found)
  {
    return self->type;
  }

  LOG_ERROR("Cant find shader.");
  return NIL_SHD_NONE;
}


/* vertex shader */

bool
nil_rsrc_shader_set_vs_src(uint32_t id, const char *src)
{
  Nil_shader *self = nullptr;
  
  const bool found           = nil_rsrc_shader_get_by_id(id, &self);
  const bool status          = (found && self->status == NIL_RSRC_STATUS_NONE);
  const bool self_assignment = (found && (self->vs_code == src));
  
  if(found && status && !self_assignment)
  {
    return nil_rsrc_copy_src_helper(&self->vs_code, src);
  }

  LOG_ERROR("Cant find or update shader.");
  return false;
}

const char*
nil_rsrc_shader_get_vs_src(uint32_t id)
{
  Nil_shader *self;
  
  const bool found = nil_rsrc_shader_get_by_id(id, &self);
  
  if(found)
  {
    return self->vs_code;
  }

  LOG_ERROR("Cant find shader.");
  return "";
}


/* geo shader */

bool
nil_rsrc_shader_set_gs_src(uint32_t id, const char *src)
{
  Nil_shader *self = nullptr;
  
  const bool found           = nil_rsrc_shader_get_by_id(id, &self);
  const bool status          = (found && self->status == NIL_RSRC_STATUS_NONE);
  const bool self_assignment = (found && (self->gs_code == src));
  
  if(found && status && !self_assignment)
  {
    return nil_rsrc_copy_src_helper(&self->gs_code, src);
  }

  LOG_ERROR("Cant find or update shader.");
  return false;
}

const char*
nil_rsrc_shader_get_gs_src(uint32_t id)
{
  Nil_shader *self;
  
  const bool found = nil_rsrc_shader_get_by_id(id, &self);
  
  if(found)
  {
    return self->gs_code;
  }

  LOG_ERROR("Cant find shader.");
  return "";
}


/* frag shader */

bool
nil_rsrc_shader_set_fs_src(uint32_t id, const char *src)
{
  Nil_shader *self = nullptr;
  
  const bool found           = nil_rsrc_shader_get_by_id(id, &self);
  const bool status          = (found && self->status == NIL_RSRC_STATUS_NONE);
  const bool self_assignment = (found && (self->fs_code == src));
  
  if(found && status && !self_assignment)
  {
    return nil_rsrc_copy_src_helper(&self->fs_code, src);
  }

  LOG_ERROR("Cant find or update shader.");
  return false;
}

const char*
nil_rsrc_shader_get_fs_src(uint32_t id)
{
  Nil_shader *self;
  
  const bool found = nil_rsrc_shader_get_by_id(id, &self);
  
  if(found)
  {
    return self->fs_code;
  }

  LOG_ERROR("Cant find shader.");
  return "";
}


/* status */

bool
nil_rsrc_shader_set_load_status(uint32_t id, Nil_resource_status new_status)
{
  Nil_shader *self = nullptr;
  
  const bool found  = nil_rsrc_shader_get_by_id(id, &self);
  
  if(found)
  {
    self->status = new_status;
    return true;
  }

  LOG_ERROR("Cant find or update shader.");
  return false;
}


Nil_resource_status
nil_rsrc_shader_get_load_status(uint32_t id)
{
  Nil_shader *self;
  
  const bool found = nil_rsrc_shader_get_by_id(id, &self);
  
  if(found)
  {
    return self->status;
  }

  LOG_ERROR("Cant find shader.");
  return NIL_RSRC_STATUS_NONE;
}



//
//namespace Nil {
//namespace Resource {
//
//
//
//
//// ----------------------------------------------------------- [ Get / Load ] --


//bool
//load(Shader &in_out)
//{
//  // -- Existing Shader -- //
//  Shader local{};
//  {
//    if(in_out.name && in_out.id)
//    {
//      find_by_index(in_out.id, &local);
//    }
//  }
//  
//  // -- Only process new or not submitted for load shaders -- //
//  if(local.status != Nil::Resource::Load_status::NONE)
//  {
//    LOG_WARNING("Can't update shader after its loaded.");
//    return false;
//  }
//  
//  // -- Remove any heap data that has been updated. -- //
//  {
//    if(local.name != in_out.name)       { free((void*)local.name);    }
//    if(local.vs_code != in_out.vs_code) { free((void*)local.vs_code); }
//    if(local.gs_code != in_out.gs_code) { free((void*)local.gs_code); }
//    if(local.fs_code != in_out.fs_code) { free((void*)local.fs_code); }
//  }
//  
//  // -- Save Shader -- //
//  {
//    // Transfer ownership //
//    {
//      bool failed = false;
//
//      // Copy the name //
//      char *cpy_name = nullptr;
//
//      if(!failed)
//      {
//        failed = !Nil_detail::copy_data_name(&cpy_name, in_out.name, malloc);
//      }
//
//      // Copy the shader code //
//      char *cpy_shd_data[3] {};
//      const char *curr_shd_data[] {in_out.vs_code, in_out.gs_code, in_out.fs_code};
//
//      LIB_ASSERT(LIB_ARRAY_SIZE(cpy_shd_data) == LIB_ARRAY_SIZE(curr_shd_data));
//
//      const size_t shd_cpy_size = LIB_ARRAY_SIZE(curr_shd_data);
//      
//      for(uint32_t i = 0; i < shd_cpy_size; ++i)
//      {
//        const bool has_data = curr_shd_data[i];
//      
//        if(!failed && has_data)
//        {
//          failed = !Nil_detail::copy_data_name(&cpy_shd_data[i], curr_shd_data[i], malloc);
//        }
//      }
//   
//      // If not failed add the new data, else delete and return.
//      if(!failed)
//      {
//        local.name    = cpy_name;
//        local.vs_code = cpy_shd_data[0];
//        local.gs_code = cpy_shd_data[1];
//        local.fs_code = cpy_shd_data[2];
//      }
//      else
//      {
//        if(cpy_name)        { free(cpy_name);        }
//        if(cpy_shd_data[0]) { free(cpy_shd_data[0]); }
//        if(cpy_shd_data[1]) { free(cpy_shd_data[1]); }
//        if(cpy_shd_data[2]) { free(cpy_shd_data[2]); }
//
//        LOG_ERROR(msg_shader_failed, in_out.name);
//        LIB_ASSERT(false);
//        
//        return false;
//      }
//    }
//    
//    // Set Outputs //
//    if(local.id == 0)
//    {
//      // Generate new id //
//      {
//        const uint32_t new_id = get_shd_data().keys.size();
//        in_out.id = new_id;
//        local.id = in_out.id;
//      }
//
//      // Normalize other outputs //
//      {
//        in_out.status = Load_status::NONE;
//        in_out.platform_resource = 0;
//        
//        local.status = in_out.status;
//        local.platform_resource = in_out.platform_resource;
//      }
//      
//      // -- Save new Shader Copy -- //
//      {
//        const uint32_t key = in_out.id;
//        get_shd_data().keys.emplace_back(key);
//        get_shd_data().shader.emplace_back(local);
//      }
//    }
//    else
//    {
//      // Update existing
//      {
//        get_shd_data().shader[local.id] = local;
//      }
//    }
//  }
//
// return true;
//}
//
//
//void
//get(size_t *count, Shader **in_out)
//{
//  *count = get_shd_data().keys.size();
//  *in_out = get_shd_data().shader.data();
//}
//
//
//// ----------------------------------------------------------------- [ Info ] --
//
//
//const char *
//get_type_name(const Shader &)
//{
//  return shader_type_name;
//}
//
//
//size_t
//shader_count()
//{
//  return get_shd_data().keys.size();
//}

//
//} // ns
//} // ns
