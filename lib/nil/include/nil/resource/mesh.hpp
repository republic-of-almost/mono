#ifndef MESH_INCLUDED_B8540088_3107_4C5A_A8B8_36D182202C3E
#define MESH_INCLUDED_B8540088_3107_4C5A_A8B8_36D182202C3E


#include <nil/fwd.hpp>
#include <nil/data/bounding_box.hpp>


/* ---------------------------------------------------------- [ Resource ] -- */


struct Nil_mesh
{
  // -- Input -- //

  const char  *name;                // Internally copied
  
  
  float       *position_vec3;       // Internally copied
  float       *normal_vec3;         // Internally copied
  float       *texture_coords_vec2; // Internally copied
  float       *color_vec4;          // Internally copied
  
  uint32_t    *index;               // Internally copied
  size_t      index_count;
  
  size_t      triangle_count;
  
  // -- Output -- //
  
  uint32_t                id;
  Nil::Data::Bounding_box bounding_box;
  Nil_resource_status     status;
  uintptr_t               platform_resource;
};


/* ------------------------------------------------- [ Resource Lifetime ] -- */


bool
nil_rsrc_mesh_initialize(Nil_ctx *ctx);


bool
nil_rsrc_mesh_destroy(Nil_ctx *ctx);


/* --------------------------------------------------- [ Resource Access ] -- */


bool
nil_rsrc_mesh_find_by_name(Nil_ctx *ctx, const char *name, Nil_mesh *out = NULL);


bool
nil_rsrc_mesh_find_by_id(Nil_ctx *ctx, uint32_t id, Nil_mesh *out = NULL);


void
nil_rsrc_mesh_get_data(Nil_ctx *ctx, size_t *out_count, Nil_mesh **out_data = NULL);


bool
nil_rsrc_mesh_get_by_id(Nil_ctx *ctx, uint32_t id, Nil_mesh **out);


/* -------------------------------------------------- [ Resource Details ] -- */


size_t
nil_rsrc_mesh_get_count(Nil_ctx *ctx);


/* ---------------------------------------------------- [ Resource Batch ] -- */


void
nil_rsrc_mesh_create_batch(Nil_ctx *ctx, Nil_mesh *in_out, size_t count, bool move = false);


/* ------------------------------------------------- [ Resource Instance ] -- */


uint32_t
nil_rsrc_mesh_create(Nil_ctx *ctx, Nil_mesh *in_out, bool move = false);


bool
nil_rsrc_mesh_destroy(Nil_ctx *ctx, uint32_t id);


/* status */


bool
nil_rsrc_mesh_set_load_status(Nil_ctx *ctx, uint32_t id, Nil_resource_status status);


Nil_resource_status
nil_rsrc_mesh_get_load_status(Nil_ctx *ctx, uint32_t id);



#endif // inc guard
