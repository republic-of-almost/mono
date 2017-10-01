#ifndef NIL_INCLUDED_AF3E5C57_06A2_419B_A843_2745F0D6F2D0
#define NIL_INCLUDED_AF3E5C57_06A2_419B_A843_2745F0D6F2D0


#include <nil/fwd.hpp>
#include <math/math.hpp> // remove


/* ---------------------------------------------------------- [ Lifetime ] -- */
/*
  Functions that deal with the lifetime of a context.
  
  The basic use is intented to be along these lines.
  
  nil_ctx_iniialize(...);
  while(nil_ctx_think(...)) {}
  nil_ctx_destroy(...);
*/


int
nil_ctx_initialize(Nil_ctx **ctx);


void
nil_ctx_destroy(Nil_ctx **ctx);


void
nil_ctx_quit_signal(Nil_ctx *ctx);


bool
nil_ctx_think(Nil_ctx *ctx);


/* ------------------------------------------------------ [ Engine State ] -- */


float
nil_ctx_get_delta_time(Nil_ctx *ctx);


size_t
nil_ctx_graph_data_count(Nil_ctx *ctx);


const uint32_t*
nil_ctx_graph_data_ids(Nil_ctx *ctx);


const uint64_t*
nil_ctx_graph_data_details(Nil_ctx *ctx);


const math::transform*
nil_ctx_graph_data_local_transforms(Nil_ctx *ctx);


const math::transform*
nil_ctx_graph_data_world_transforms(Nil_ctx *ctx);


/* ----------------------------------------------------------- [ Aspects ] -- */


void
nil_ctx_add_aspect(Nil_ctx *ctx, Nil_aspect aspect);


size_t
nil_ctx_aspect_count(Nil_ctx *ctx);


#endif // inc guard
