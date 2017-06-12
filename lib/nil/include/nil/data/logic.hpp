#ifndef LOGIC_INCLUDED_AC052605_9926_4B8E_BFAC_06B52EA74A5C
#define LOGIC_INCLUDED_AC052605_9926_4B8E_BFAC_06B52EA74A5C


#include <nil/fwd.hpp>
#include <stdint.h>


namespace Nil {

using generic_callback_fn = void(*)(Nil::Node node, uintptr_t user_data);

namespace Data {



struct Logic
{
  uint32_t      type;
  uint32_t      logic_id;
  
  uintptr_t     user_data;
  
  generic_callback_fn     startup;
  generic_callback_fn     think_01;
  generic_callback_fn     think_02;
  generic_callback_fn     think_03;
  generic_callback_fn     message;
  generic_callback_fn     shutdown;
};


void
get(const Node &node, Logic &out);


void
set(Node &node, const Logic &in);


bool
has_logic(const Node &node);


uint64_t
get_type_id(const Logic &in);


} // ns
} // ns


#endif // inc guard
