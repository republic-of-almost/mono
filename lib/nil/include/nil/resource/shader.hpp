#ifndef SHADER_INCLUDED_69FC0F54_CDC9_4734_ACE5_E5369C3A3E57
#define SHADER_INCLUDED_69FC0F54_CDC9_4734_ACE5_E5369C3A3E57


#include <nil/fwd.hpp>


/* -------------------------------------------------------------- [ Type ] -- */


struct Nil_shader
{
  /* input */

  const char          *name;
  Nil_shader_type     type;
  const char          *vs_code;
  const char          *gs_code;
  const char          *fs_code;
  
  /* output */
  
  Nil_resource_status status;
  uintptr_t           platform_resource;
  uint32_t            id;
};


/* ------------------------------------------------------------ [ Static ] -- */
/*
  These functions operate on the collective shader data.
*/

/* rsrc lifetime */

bool
nil_rsrc_shader_initialize();

bool
nil_rsrc_shader_destroy();


/* search / access */

bool
nil_rsrc_shader_find_by_name(const char *name, Nil_shader *out = NULL);

bool
nil_rsrc_shader_find_by_id(uint32_t id, Nil_shader *out = NULL);

void
nil_rsrc_shader_get_data(size_t *out_count, Nil_shader **out_data = NULL);

bool
nil_rsrc_shader_get_by_id(uint32_t id, Nil_shader **out);


/* details */

size_t
nil_rsrc_shader_get_count();


/* batch */

void
nil_rsrc_shader_create_batch(Nil_shader *in, size_t count, bool move = false);


/* ---------------------------------------------------------- [ Instance ] -- */
/*
  These functions operator on an instance of a shader.
*/

/* lifetime */

uint32_t
nil_rsrc_shader_create(Nil_shader *shd, bool move = false);

bool
nil_rsrc_shader_destroy(uint32_t id);


/* type */

bool
nil_rsrc_shader_set_type(uint32_t id, Nil_shader_type type);

Nil_shader_type
nil_rsrc_shader_get_type(uint32_t id);


/* vertex shader */

bool
nil_rsrc_shader_set_vs_src(uint32_t id, const char *src);

const char*
nil_rsrc_shader_get_vs_src(uint32_t id);


/* geo shader */

bool
nil_rsrc_shader_set_gs_src(uint32_t id, const char *src);

const char*
nil_rsrc_shader_get_gs_src(uint32_t id);


/* frag shader */

bool
nil_rsrc_shader_set_fs_src(uint32_t id, const char *src);

const char*
nil_rsrc_shader_get_fs_src(uint32_t id);


/* status */

bool
nil_rsrc_shader_set_load_status(uint32_t id, Nil_resource_status status);

Nil_resource_status
nil_rsrc_shader_get_load_status(uint32_t id);


#endif // inc guard
