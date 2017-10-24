#ifndef CODEX_INCLUDED_965A1D8B_A82F_4BC6_877F_336AF9CBB191
#define CODEX_INCLUDED_965A1D8B_A82F_4BC6_877F_336AF9CBB191


#include <inttypes.h>
#include <stddef.h>
#include <stdbool.h>


#ifdef __cplusplus
extern "C" {
#endif


/* ------------------------------------------------------------- [ Codex ] -- */
/*
  The Codex is a data store. You create object types and attach properties to
  those objects. You can attach any number of properties to an object. To create
  object instances you must first create a 'instance', after which you can
  attach any number of 'object_types' to that instance
*/


struct Codex_ctx;


/* ---------------------------------------------------- [ Codex Lifetime ] -- */


bool        codex_create(struct Codex_ctx **c);
void        codex_destroy(struct Codex_ctx **c);


/* ---------------------------------------------------- [ Codex Instance ] -- */


uint32_t    codex_instance_create(struct Codex_ctx *c);
bool        codex_instance_destroy(struct Codex_ctx *c, uint32_t inst_id);
size_t      codex_instance_count(const struct Codex_ctx *c);


/* ------------------------------------------------ [ Codex Object Type  ] -- */


uint32_t    codex_object_type_create(struct Codex_ctx *c, const char *name);
size_t      codex_object_type_count(const struct Codex_ctx *c);


/* ------------------------------------- [ Codex Object Type Properties  ] -- */


enum {
  CODEX_PROP_BOOL,
  CODEX_PROP_STRING,
  CODEX_BYTE,
  CODEX_PTR,

  CODEX_PROP_INT, CODEX_PROP_INT2, CODEX_PROP_INT3,CODEX_PROP_INT4,

  CODEX_PROP_FLOAT, CODEX_PROP_FLOAT2, CODEX_PROP_FLOAT3,CODEX_PROP_FLOAT4,
  CODEX_PROP_FLOAT5, CODEX_PROP_FLOAT6, CODEX_PROP_FLOAT7, CODEX_PROP_FLOAT8,
  CODEX_PROP_FLOAT9, CODEX_PROP_FLOAT10, CODEX_PROP_FLOAT11, CODEX_PROP_FLOAT12,
  CODEX_PROP_FLOAT13, CODEX_PROP_FLOAT14, CODEX_PROP_FLOAT15, CODEX_PROP_FLOAT16,
};


uint32_t    codex_property_create(struct Codex_ctx *c, uint32_t obj_id, uint32_t prop_type, const char *name);
size_t      codex_property_count(struct Codex_ctx *c, uint32_t obj_id);


#ifdef __cplusplus
}
#endif


#endif /* inc guard */
