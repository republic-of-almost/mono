/*
  Developer
  --
  This data is to be used at the developers discretion.
  You can store pointers or other custom information here.
*/
#ifndef DEVELOPER_INCLUDED_B9198F1D_13AB_44EF_BFDF_8FC95C73471F
#define DEVELOPER_INCLUDED_B9198F1D_13AB_44EF_BFDF_8FC95C73471F


#include <nil/fwd.hpp>
#include <stdint.h>


namespace Nil {
namespace Data {


struct Developer
{
  uint32_t type_id;
  uintptr_t aux_01;
  uintptr_t aux_02;
  uintptr_t aux_03;
  uintptr_t aux_04;
};


void
get(const Node &node, Developer &out);


void
set(Node &node, const Developer &in);


bool
has_developer(const Node &node);


uint64_t
get_type_id(const Developer &in);


} // ns
} // ns


#endif // inc guard
