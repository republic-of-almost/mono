#ifndef AUDIO_INCLUDED_A9E9DFF6_0B6A_45AF_BB36_79A82AB30911
#define AUDIO_INCLUDED_A9E9DFF6_0B6A_45AF_BB36_79A82AB30911


#include <nil/fwd.hpp>


namespace Nil {
namespace Resource {


// ------------------------------------------------------------- [ Resource ] --
/*!
  Audio
*/
struct Audio
{
  const char *name;
  enum { FILENAME } data_type;
  uintptr_t         data;
  size_t            data_size;
  
  // -- Output -- //
  
  enum { PENDING, LOADED, FAILED, STATUS_COUNT } status;
  uintptr_t platform_resource;
  uint32_t id;
};


// ------------------------------------------------------------- [ Get Data ] --


bool
find_by_name(const char *name, Audio &out);


void
get(size_t *count, Audio **out);


// ----------------------------------------------------------------- [ Load ] --


bool
load(Audio &in);


// ----------------------------------------------------------------- [ Info ] --


const char *
get_type_name(const Audio &in);



} // ns
} // ns


#endif // inc guard
