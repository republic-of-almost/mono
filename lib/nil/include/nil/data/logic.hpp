#ifndef LOGIC_INCLUDED_AC052605_9926_4B8E_BFAC_06B52EA74A5C
#define LOGIC_INCLUDED_AC052605_9926_4B8E_BFAC_06B52EA74A5C


#include <nil/fwd.hpp>
#include <stdint.h>
#include <stddef.h>



namespace Nil {

//using generic_callback_fn = void(*)(uint32_t node, uintptr_t user_data);

namespace Data {


// ----------------------------------------------------------------- [ Data ] --


struct Logic
{
  uint32_t      type;
  uint32_t      logic_id;
  
  uintptr_t     user_data;
  
  uintptr_t     update_func;
  uintptr_t     message_func;
  
  
};


// -------------------------------------------------------------- [ Get/Set ] --


void
get(size_t *count, Logic **data);


void
get(const Node &node, Logic &out);


void
set(Node &node, const Logic &in);


void
remove_logic(Node &node);


// ----------------------------------------------------------------- [ Info ] --


bool
has_logic(const Node &node);


bool
has(const Node &node, const Logic &data);


uint64_t
get_type_id(const Logic &in);


const char*
get_type_name(const Logic &in);


size_t
logic_count();


// ---------------------------------------------------------- [ Data Events ] --


void
events(const uint32_t data, size_t *count, Logic **out_data, Node **out_node);


} // ns
} // ns


#endif // inc guard
