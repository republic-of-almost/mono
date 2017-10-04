#ifndef AUDIO_INCLUDED_A9E9DFF6_0B6A_45AF_BB36_79A82AB30911
#define AUDIO_INCLUDED_A9E9DFF6_0B6A_45AF_BB36_79A82AB30911


#include <nil/fwd.hpp>


// ------------------------------------------------------------- [ Resource ] --
/*
  Audio
*/
struct Nil_audio_src
{
  /* input */
  
  const char          *name;
  Nil_data_type       data_type;
  uintptr_t           data;
  size_t              data_size;
  
  Nil_audio_src_type  audio_type;
  
  /* output */
  
  Nil_resource_status status;
  uintptr_t           platform_resource;
  uint32_t            id;
};


/* ------------------------------------------------- [ Resource Lifetime ] -- */


bool
nil_rsrc_audio_src_initialize(Nil_ctx *ctx);


bool
nil_rsrc_audio_src_destroy(Nil_ctx *ctx);


/* --------------------------------------------------- [ Resource Access ] -- */


bool
nil_rsrc_audio_src_find_by_name(Nil_ctx *ctx, const char *name, Nil_audio_src *out = NULL);


bool
nil_rsrc_audio_src_find_by_id(Nil_ctx *ctx, uint32_t id, Nil_audio_src *out = NULL);


void
nil_rsrc_audio_src_get_data(Nil_ctx *ctx, size_t *out_count, Nil_audio_src **out_data = NULL);


bool
nil_rsrc_audio_src_get_by_id(Nil_ctx *ctx, uint32_t id, Nil_audio_src **out);


/* -------------------------------------------------- [ Resource Details ] -- */


size_t
nil_rsrc_audio_src_get_count(Nil_ctx *ctx);


/* ---------------------------------------------------- [ Resource Batch ] -- */


void
nil_rsrc_audio_src_create_batch(Nil_ctx *ctx, Nil_audio_src *in_out, size_t count, bool move = false);


/* ------------------------------------------------- [ Resource Instance ] -- */


uint32_t
nil_rsrc_audio_src_create(Nil_ctx *ctx, Nil_audio_src *in_out, bool move = false);


bool
nil_rsrc_audio_src_destroy(Nil_ctx *ctx, uint32_t id);


/* status */


bool
nil_rsrc_audio_src_set_load_status(Nil_ctx *ctx, uint32_t id, Nil_resource_status status);


Nil_resource_status
nil_rsrc_audio_src_get_load_status(Nil_ctx *ctx, uint32_t id);


#endif // inc guard
