#ifndef MATERIAL_INCLUDED_F58F218E_C94E_49D4_9CA3_34CBD7A35E26
#define MATERIAL_INCLUDED_F58F218E_C94E_49D4_9CA3_34CBD7A35E26


#include <nil/fwd.hpp>


// ------------------------------------------------------------- [ Resource ] --


struct Nil_material
{
  /* input */

  const char      *name;
  uint32_t        color;
  uint32_t        texture_01;
  uint32_t        texture_02;
  uint32_t        texture_03;
  uint32_t        shader_id;
  
  /* output */
  
  Nil_resource_status   status;
  uint32_t              id;
};


/* ------------------------------------------------- [ Resource Lifetime ] -- */


bool
nil_rsrc_material_initialize(Nil_ctx *ctx);


bool
nil_rsrc_material_destroy(Nil_ctx *ctx);


/* --------------------------------------------------- [ Resource Access ] -- */


bool
nil_rsrc_material_find_by_name(Nil_ctx *ctx, const char *name, Nil_material *out = NULL);


bool
nil_rsrc_material_find_by_id(Nil_ctx *ctx, uint32_t id, Nil_material *out = NULL);


void
nil_rsrc_material_get_data(Nil_ctx *ctx, size_t *out_count, Nil_material **out_data);


bool
nil_rsrc_material_get_by_id(Nil_ctx *ctx, uint32_t id, Nil_material **out);


/* -------------------------------------------------- [ Resource Details ] -- */


size_t
nil_rsrc_material_get_count(Nil_ctx *ctx);


/* ---------------------------------------------------- [ Resource Batch ] -- */


void
nil_rsrc_material_create_batch(Nil_ctx *ctx, Nil_material *in_out, size_t count, bool move = false);


/* ------------------------------------------------- [ Resource Instance ] -- */


uint32_t
nil_rsrc_material_create(Nil_ctx *ctx, Nil_material *in_out, bool move = false);


bool
nil_rsrc_material_destroy(Nil_ctx *ctx, uint32_t id);


/* status */


bool
nil_rsrc_material_set_load_status(Nil_ctx *ctx, uint32_t id, Nil_resource_status status);


Nil_resource_status
nil_rsrc_material_get_load_status(Nil_ctx *ctx, uint32_t id);


#endif // inc guard
