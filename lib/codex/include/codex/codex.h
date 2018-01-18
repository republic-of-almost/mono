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

typedef enum codex_property {

  LOCAL_POSITION_F3,
  LOCAL_SCALE_F3,
  LOCAL_ROTATION_F4,

} codex_property;

typedef struct codex_property_writer codex_property_writer;


/* ---------------------------------------------------- [ Codex Lifetime ] -- */


void
codex_create(
  struct codex_ctx            **ctx);


void
codex_destroy(
  struct codex_ctx            **ctx);


/* ------------------------------------------------------ [ Codex Object ] -- */


unsigned
codex_object_create(
  struct codex_ctx            *ctx);


CODEX_BOOL
codex_object_destroy(
  struct codex_ctx            *ctx,
  unsigned                    obj_id);


unsigned
codex_object_count(
  struct codex_ctx            *ctx);


CODEX_BOOL
codex_object_exists(
  struct codex_ctx            *ctx,
  unsigned                    obj_id);


/* -------------------------------------------------- [ Codex Properties ] -- */


void
codex_property_read_float(
  struct codex_ctx            *ctx,
  unsigned                    obj_id,
  codex_property              property,
  float                       *out_data);


codex_property_writer*
codex_property_get_writer(
  struct codex_ctx            *ctx,
  unsigned                    obj_id);


void
codex_property_write_float(
  codex_property_writer       *writer,
  codex_property              property,
  const float                 *in_data);


void
codex_property_commit_writer(
  codex_property_writer       *writer);


#ifdef __cplusplus
} /* extern */
#endif


#endif /* inc guard */