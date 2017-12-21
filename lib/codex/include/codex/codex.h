#ifndef CODEX_INCLUDED_24D9849C_C842_46F8_8BA7_5DCE9B1883DD
#define CODEX_INCLUDED_24D9849C_C842_46F8_8BA7_5DCE9B1883DD


#ifdef __cplusplus
extern "C" {
#endif


/* ------------------------------------------------------- [ Codex Types ] -- */


struct codex_ctx;
typedef int CODEX_BOOL;

#define CODEX_TRUE 1
#define CODEX_FALSE 0


/* ---------------------------------------------------- [ Codex Lifetime ] -- */


void
codex_create(
  struct codex_ctx **ctx);


void
codex_destroy(
  struct codex_ctx **ctx);


/* ------------------------------------------------------ [ Codex Object ] -- */


typedef void(*codex_object_created_callback_fn)(unsigned obj_ids[], unsigned count, void *user_data);
typedef void(*codex_object_destroyed_callback_fn)(unsigned obj_ids[], unsigned count, void *user_data);


struct codex_callbacks
{
  codex_object_created_callback_fn created;
  void *created_user_data;

  codex_object_destroyed_callback_fn destroyed;
  void *destroyed_user_data;
};


void
codex_object_callbacks(
  struct codex_ctx *ctx,
  struct codex_callbacks *callbacks);


unsigned
codex_object_create(
  struct codex_ctx *ctx);


CODEX_BOOL
codex_object_destroy(
  struct codex_ctx *ctx,
  unsigned obj_id);


unsigned
codex_object_count(
  struct codex_ctx *ctx);


/* ---------------------------------------- [ Codex Object Relationships ] -- */


CODEX_BOOL
codex_object_set_parent(
  struct codex_ctx *ctx,
  unsigned this_id,
  unsigned parent_id);


void
codex_object_get_children(
  struct codex_ctx *ctx,
  unsigned *obj_id,
  unsigned *count);


/* -------------------------------------------------- [ Codex Properties ] -- */


void
codex_property_get();


#ifdef __cplusplus
} /* extern */
#endif


#endif /* inc guard */