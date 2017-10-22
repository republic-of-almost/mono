#include <codex/codex.h>


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
  uint32_t property_id;

  uint32_t *instance_id;
  uint8_t  *data;
};


struct Codex_object_type
{
  uint32_t object_id;

  uint32_t *properties;
};


struct Codex_ctx
{
  uint32_t instance_id_counter;
  uint32_t object_id_counter;

  uint32_t          *instance_ids;
  struct Codex_object_type *object_types;
  uint32_t          *object_instances;
};


/* ---------------------------------------------------- [ Codex Lifetime ] -- */


bool
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
      return false;
    }

    /* defaults */
    const size_t instance_count = 1024;
    const size_t instance_bytes = instance_count * sizeof(uint32_t);

  }

  /* Created instance */
  *c = new_ctx;
  return true;
}


void
codex_destroy(struct Codex_ctx **c)
{
  /* free */
  if(*c != NULL)
  {

    CODEX_FREE((*c));
  }
}


/* ---------------------------------------------------- [ Codex Instance ] -- */


uint32_t
codex_instance_create(struct Codex_ctx *c)
{
  /* param check */
  {
    CODEX_ASSERT(c);
  }

  /* add new instance */
  {

  }
  
  return 0;
}


void
codex_instance_destroy(struct Codex_ctx *c, uint32_t inst_id)
{

}


/* ------------------------------------------------- [ Codex Object Type ] -- */


uint32_t
codex_object_type_create(struct Codex_ctx *c, const char *name)
{

}


void
codex_object_type_attach(struct Codex_ctx *c, uint32_t obj_id, uint32_t inst_id)
{

}


void
codex_object_type_detach(struct Codex_ctx *c, uint32_t obj_id, uint32_t inst_id)
{

}


/* -------------------------------------- [ Codex Object Type Properties ] -- */


uint32_t
codex_property_create(struct Codex_ctx *c, uint32_t obj_id, uint32_t prop_type, const char *name)
{

}


size_t
codex_property_count(struct Codex_ctx *c, uint32_t obj_id)
{

}


/* ------------------------------------------------------ [ Codex Config ] -- */


#undef CODEX_MALLOC
#undef CODEX_FREE
#undef CODEX_MEMZERO
#undef CODEX_MEMCPY
#undef CODEX_ASSERT
