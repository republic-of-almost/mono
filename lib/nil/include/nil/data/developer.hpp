/*
  Developer
  --
  This data is to be used at the developers discretion.
  You can store pointers or other custom information here.
*/
#ifndef DEVELOPER_INCLUDED_B9198F1D_13AB_44EF_BFDF_8FC95C73471F
#define DEVELOPER_INCLUDED_B9198F1D_13AB_44EF_BFDF_8FC95C73471F


#include <nil/fwd.hpp>


namespace Nil {
namespace Data {


// ----------------------------------------------------------------- [ Data ] --


struct Developer
{
  uint32_t type_id;
  uintptr_t aux_01;
  uintptr_t aux_02;
  uintptr_t aux_03;
  uintptr_t aux_04;
};


// -------------------------------------------------------------- [ Get/Set ] --


void
get(size_t *count, Developer **data);


void
get(const Node &node, Developer &out);


void
set(Node &node, const Developer &in);


void
remove_developer(Node &node);


// ----------------------------------------------------------------- [ Info ] --


bool
has_developer(const Node &node);


bool
has(const Node &node, const Developer &data);


uint64_t
get_type_id(const Developer &in);


const char*
get_type_name(const Developer &in);


size_t
developer_count();


// ---------------------------------------------------------- [ Data Events ] --


void
events(const uint32_t data, size_t *count, Developer **out_data, Node **out_node);


} // ns
} // ns


#endif // inc guard
