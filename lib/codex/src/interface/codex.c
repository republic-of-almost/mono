#include <codex/codex.h>
#include <common/array.h>


/* ------------------------------------------------------ [ Codex Config ] -- */


#ifndef CODEX_MALLOC
#include <stdlib.h>
#define CODEX_MALLOC(bytes) malloc(bytes)
#endif


#ifndef CODEX_FREE
#include <stdlib.h>
#define CODEX_FREE(addr) free(addr)
#endif


#ifndef CODEX_MEMZERO
#include <string.h>
#define CODEX_MEMZERO(addr, size) memset(addr, 0, size)
#endif


#ifndef CODEX_MEMCPY
#include <string.h>
#define CODEX_MEMCPY(src, dest, size) memcpy(src, dest, size)
#endif


#ifndef CODEX_ASSERT
#include <assert.h>
#define CODEX_ASSERT(expr) assert(expr)
#endif


/* ------------------------------------------------------- [ Codex Types ] -- */


struct Codex_property
{
  const char *name;
  uint32_t property_id;

  uint32_t *instance_id;
  uint8_t  *data;
};


struct Codex_object_type
{
  const char *name;
  uint32_t object_id;
  uint32_t *properties;
};


struct Codex_instance
{
  const char *name;
  uint64_t object_ids;
};


struct Codex_ctx
{
  uint32_t                  instance_id_counter;
  
  uint32_t                  *instance_ids;
  
  struct Codex_instance     *instances;
  struct Codex_object_type  *object_types;
  struct Codex_property     *properties;
};


/* ---------------------------------------------------- [ Codex Lifetime ] -- */


Codex_bool
codex_create(struct Codex_ctx **c)
{
  /* param check */
  {
    CODEX_ASSERT(*c);
  }

  /* create new context */
  struct Codex_ctx *new_ctx = NULL;
  {
    const size_t bytes = sizeof(struct Codex_ctx);
    new_ctx = (struct Codex_ctx *)CODEX_MALLOC(bytes);

    CODEX_ASSERT(new_ctx); // Failed to alloc
    CODEX_MEMZERO(new_ctx, bytes);

    if(!new_ctx)
    {
      return CODEX_FALSE;
    }

    /* defaults */
    {
      codex_array_create(new_ctx->instance_ids,     1024);
      codex_array_create(new_ctx->instances,        1024);
      codex_array_create(new_ctx->object_types,     64);
      codex_array_create(new_ctx->properties,       512);
    }
  }

  /* Created instance */
  *c = new_ctx;
  return CODEX_TRUE;
}


void
codex_destroy(struct Codex_ctx **c)
{
  /* free */
  if(*c != NULL)
  {
    codex_array_destroy((*c)->instance_ids);
    codex_array_destroy((*c)->instances);
    codex_array_destroy((*c)->object_types);
    codex_array_destroy((*c)->properties);
  
    CODEX_FREE((*c));
    
    *c = NULL;
  }
}


/* ---------------------------------------------------- [ Codex Instance ] -- */


uint32_t
codex_instance_create(struct Codex_ctx *c)
{
  /* param and state check */
  {
    CODEX_ASSERT(c);
    CODEX_ASSERT(codex_array_size(c->instance_ids) == codex_array_size(c->instances));
  }

  /* add new instance */
  {
    uint32_t new_instance = ++(c->instance_id_counter);
    codex_array_push(c->instance_ids, new_instance);
    
    struct Codex_instance new_instance_data;
    CODEX_MEMZERO(&new_instance_data, sizeof(new_instance_data));
    
    codex_array_push(c->instances, new_instance_data);
    
    return new_instance;
  }
  
  return 0;
}


Codex_bool
codex_instance_destroy(struct Codex_ctx *c, uint32_t inst_id)
{
  /* param and state check */
  {
    CODEX_ASSERT(c);
    CODEX_ASSERT(inst_id);
    CODEX_ASSERT(codex_array_size(c->instance_ids) == codex_array_size(c->instances));
  }

  const size_t count = codex_array_size(c->instance_ids);
  size_t i;
  
  for(i = 0; i < count; ++i)
  {
    if(c->instance_ids[i] == inst_id)
    {
      codex_array_erase(c->instance_ids, i);
      codex_array_erase(c->instances, i);
      
      CODEX_ASSERT(codex_array_size(c->instance_ids) == codex_array_size(c->instances));
      
      return CODEX_TRUE;
    }
  }
  
  return CODEX_FALSE;
}


size_t
codex_instance_count(const struct Codex_ctx *c)
{
  /* param and state check */
  {
    CODEX_ASSERT(c);
    CODEX_ASSERT(codex_array_size(c->instance_ids) == codex_array_size(c->instances));
  }

  return codex_array_size(c->instance_ids);
}


void
codex_instance_add_object(struct Codex_ctx *c, uint32_t inst_id, uint32_t obj_id)
{
  /* param check */
  {
    CODEX_ASSERT(c);
    CODEX_ASSERT(inst_id);
    CODEX_ASSERT(obj_id);
  }
  
  /* find index and add flag */
  {
    const size_t count = codex_array_size(c->instance_ids);
    size_t i;
    
    const uint64_t flag = 1 << obj_id;
    
    for(i = 0; i < count; ++i)
    {
      if(c->instance_ids[i] == inst_id)
      {
        c->instances[i].object_ids |= flag;
      }
    }
  }
}


/* ------------------------------------------------- [ Codex Object Type ] -- */


uint32_t
codex_object_type_create(struct Codex_ctx *c, const char *name)
{
  /* param check */
  {
    CODEX_ASSERT(c);
    CODEX_ASSERT(name);
    CODEX_ASSERT(strlen(name));
  }

  /* search for names */
  {
    size_t count = codex_array_size(c->object_types);
    size_t i;
    
    for(i = 0; i < count; ++i)
    {
      const Codex_bool name_match = strcmp(c->object_types[i].name, name) == 0;
    
      if(name_match)
      {
        CODEX_ASSERT(CODEX_FALSE); // name exists
        return 0;
      }
    }
  }
  
  /* add new type */
  {
    struct Codex_object_type new_obj;
    CODEX_MEMZERO(&new_obj, sizeof(new_obj));
    
    char *name_cpy = (char *)CODEX_MALLOC(strlen(name) + 1);
    strcpy(name_cpy, name);
    
    new_obj.name       = name_cpy;
    new_obj.object_id  = codex_array_size(c->object_types) + 1;
    
    codex_array_create(new_obj.properties, 32);
    
    codex_array_push(c->object_types, new_obj);
    
    return new_obj.object_id;
  }
  
  return 0;
}


size_t
codex_object_type_count(const struct Codex_ctx *c)
{
  /* param check */
  {
    CODEX_ASSERT(c);
  }
  
  return codex_array_size(c->object_types);
}


/* -------------------------------------- [ Codex Object Type Properties ] -- */


uint32_t
codex_property_create(struct Codex_ctx *c, uint32_t obj_id, uint32_t prop_type, const char *name)
{
  /* param check */
  {
    CODEX_ASSERT(c);
    CODEX_ASSERT(obj_id);
    CODEX_ASSERT(prop_type);
    CODEX_ASSERT(name != NULL);
    CODEX_ASSERT(strlen(name) > 0);
  }
  
  /* check to see if name exists */
  {
    const size_t o_index = obj_id - 1;
    const uint32_t *prop_ids = c->object_types[o_index].properties;
    const size_t prop_count = codex_array_size(prop_ids);
    
    size_t i = 0;
    
    for(i = 0; i < prop_count; ++i)
    {
      const size_t p_index = prop_ids[i] - 1;
      struct Codex_property *prop = &c->properties[p_index];
      const Codex_bool name_exists = (strcmp(prop->name, name) == 0);
      
      if(name_exists)
      {
        return 0;
      }
    }
  }
  
  /* add new property and to object */
  {
    struct Codex_property prop;
    prop.property_id = codex_array_size(c->properties) + 1;
    
    char *prop_name = CODEX_MALLOC(strlen(name) + 1);
    prop.name = prop_name;
    strcpy(prop_name, name);
    
    codex_array_push(c->properties, prop);
    
    const size_t obj_index = obj_id - 1;
    struct Codex_object_type *obj = &c->object_types[obj_index];
    
    codex_array_push(obj->properties, prop.property_id);
    
    return codex_array_size(c->properties);
  }
  
  return 0;
}


size_t
codex_property_count(struct Codex_ctx *c, uint32_t obj_id)
{
  return codex_array_size(c->properties);
}


/* ------------------------------------------------------ [ Codex Config ] -- */


#undef CODEX_MALLOC
#undef CODEX_FREE
#undef CODEX_MEMZERO
#undef CODEX_MEMCPY
#undef CODEX_ASSERT
