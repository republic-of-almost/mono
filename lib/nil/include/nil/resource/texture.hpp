#ifndef TEXTURE_INCLUDED_56366719_6675_4A06_AEA8_5615D7AA8B07
#define TEXTURE_INCLUDED_56366719_6675_4A06_AEA8_5615D7AA8B07


#include <nil/fwd.hpp>


/* ---------------------------------------------------------- [ Resource ] -- */


struct Nil_texture
{  
  // -- Input --//

  const char        *name;      // Internally copied
  Nil_data_type     data_type;
  uintptr_t         data;       // Internally copied
  size_t            data_size;
  
  // -- Input / Output -- //
  
  uint32_t width;       // If data_type == DATA then this is input.
  uint32_t height;      // If data_type == DATA then this is input.
  uint32_t depth;       // If data_type == DATA then this is input.
  uint32_t components;  // If data_type == DATA then this is input.
  
  // -- Output -- //
  
  Nil_resource_status status;
  uintptr_t           platform_resource;
  uint32_t            id;
};


/* ------------------------------------------------- [ Resource Lifetime ] -- */


bool
nil_rsrc_texture_initialize(Nil_ctx *ctx);


bool
nil_rsrc_texture_destroy(Nil_ctx *ctx);


/* --------------------------------------------------- [ Resource Access ] -- */


bool
nil_rsrc_texture_find_by_name(Nil_ctx *ctx, const char *name, Nil_texture *out);


bool
nil_rsrc_texture_find_by_id(Nil_ctx *ctx, uint32_t id, Nil_texture *out);


void
nil_rsrc_texture_get_data(Nil_ctx *ctx, size_t *out_count, Nil_texture **out_data);


bool
nil_rsrc_texture_get_by_id(Nil_ctx *ctx, uint32_t id, Nil_texture **out);


/* -------------------------------------------------- [ Resource Details ] -- */


size_t
nil_rsrc_texture_get_count(Nil_ctx *ctx);


/* ---------------------------------------------------- [ Resource Batch ] -- */


void
nil_rsrc_texture_create_batch(Nil_ctx *ctx, Nil_texture *in, size_t count, bool move = false);


/* ------------------------------------------------- [ Resource Instance ] -- */


uint32_t
nil_rsrc_texture_create(Nil_ctx *ctx, Nil_texture *shd, bool move = false);


bool
nil_rsrc_texture_destroy(Nil_ctx *ctx, uint32_t id);


/* status */


bool
nil_rsrc_texture_set_load_status(Nil_ctx *ctx, uint32_t id, Nil_resource_status status);


Nil_resource_status
nil_rsrc_texture_get_load_status(Nil_ctx *ctx, uint32_t id);


#endif // inc guard
