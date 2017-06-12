#ifndef WINDOW_INCLUDED_9EC4B4E2_D0F9_4990_B4F8_3EB523EBD2DE
#define WINDOW_INCLUDED_9EC4B4E2_D0F9_4990_B4F8_3EB523EBD2DE


#include <nil/fwd.hpp>
#include <stdint.h>


namespace Nil {
namespace Data {


struct Window
{
  uint32_t width;
  uint32_t height;
  bool fullscreen;
  char title[32];
  
  uintptr_t user_data;
};


void
get(const Node &node, Window &out);


void
set(Node &node, const Window &in);


bool
has_window(const Node &node);


uint64_t
get_type_id(const Window &in);


} // ns
} // ns


#endif // inc guard
