#ifndef OBJECT_INCLUDED_EC4C4710_83A5_4AC8_AF58_EAD6699152E9
#define OBJECT_INCLUDED_EC4C4710_83A5_4AC8_AF58_EAD6699152E9


#include <roa/fundamental.hpp>


/* --------------------------------------------------- [ Object Lifetime ] -- */
/*
  Create and destroy objects.
  When an object is destroyed its data is also destroyed.
*/


uint64_t      roa_object_create();
void          roa_object_destroy();


/* ------------------------------------------------------ [ Object State ] -- */
/*
  Various state checkers for an object.
*/


bool          roa_object_is_valid();
uint32_t      roa_object_instance(uint64_t id);


/* ---------------------------------------------- [ Object Relationships ] -- */
/*
  An object is a tree like structure it may have any number of children.
  Any data returned from _get_children functions is only valid for one frame.
*/


uint64_t      roa_object_get_parent(uint64_t id);
void          roa_object_set_parent(uint64_t this_id, uint64_t new_parent);

size_t        roa_object_get_child_count(uint64_t id);
void          roa_object_get_children(uint64_t id, uint64_t **o_ids, size_t *o_count);
uint64_t      roa_object_get_child(uint64_t id, size_t index);


/* ------------------------------------------------- [ Object Attributes ] -- */
/*
  An object may have various settings such as names and user data.
  Any data returned from _get_children_ functions is only valid for one frame.
*/


const char *  roa_object_get_name(uint64_t id);
void          roa_object_set_name(uint64_t id, const char *name);
void          roa_object_get_children_names(uint64_t id, const char **o_names, size_t *o_count);

uintptr_t     roa_object_get_user_data(uint64_t id);
void          roa_object_set_user_data(uint64_t id, uintptr_t user_data);
void          roa_object_get_children_user_data(uint64_t id, uintptr_t **o_data, size_t *o_count);


/* ------------------------------------------------------- [ Object Data ] -- */
/*
  You can attach other valid data to an object.
*/

void          roa_object_set_data(uint64_t id, uint64_t data);
uint64_t      roa_object_get_data(uint64_t id, uint32_t data_type);
void          roa_object_remove_data(uint64_t id, uint32_t data_type);


#endif // inc guard
