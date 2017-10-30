#ifndef OBJECT_INCLUDED_D7CB1025_F5A6_4EC1_9A3E_F53CEED4F197
#define OBJECT_INCLUDED_D7CB1025_F5A6_4EC1_9A3E_F53CEED4F197


#include <coretech/fundamental.h>


#ifdef __cplusplus
extern "C" {
#endif


/* --------------------------------------------------- [ Object Lifetime ] -- */


uint64_t      ct_object_create();
void          ct_object_destroy(uint64_t obj_id);


/* ------------------------------------------------- [ Object Attributes ] -- */


void          ct_object_set_name(uint64_t obj_id, const char *name);
const char *  ct_object_get_name(uint64_t obj_id);


#ifdef __cplusplus
}
#endif


#endif /* inc guard */
