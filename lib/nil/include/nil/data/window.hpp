#ifndef WINDOW_INCLUDED_9EC4B4E2_D0F9_4990_B4F8_3EB523EBD2DE
#define WINDOW_INCLUDED_9EC4B4E2_D0F9_4990_B4F8_3EB523EBD2DE


#include <nil/fwd.hpp>
#include <stdint.h>
#include <stddef.h>


namespace Nil {
namespace Data {


// ----------------------------------------------------------------- [ Data ] --


struct Window
{
  uint32_t  width;
  uint32_t  height;
  bool      fullscreen;
  char      title[32];
  
  uintptr_t user_data;
  
  // Output //
  
  enum { NONE, OGL }  type;
  uint32_t      major;
  uint32_t      minor;
};


// -------------------------------------------------------------- [ Get/Set ] --


void
get(size_t *out_count, Window **out_resource);


void
get(const Node &node, Window &out);


void
set(Node &node, const Window &in);


void
remove_window(Node &node);


// ----------------------------------------------------------------- [ Info ] --


bool
has_window(const Node &node);


bool
has(const Node &node, const Window &data);


uint64_t
get_type_id(const Window &in);


const char*
get_type_name(const Window &in);


size_t
window_count();


// ---------------------------------------------------------- [ Data Events ] --


void
events(const uint32_t data, size_t *count, Window **out_data, Node **out_node);


} // ns
} // ns


#endif // inc guard
