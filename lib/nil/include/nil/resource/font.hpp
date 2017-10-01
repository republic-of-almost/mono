#ifndef FONT_INCLUDED_FAE30313_D649_4821_B300_7C4AA5FF4885
#define FONT_INCLUDED_FAE30313_D649_4821_B300_7C4AA5FF4885


#include <nil/fwd.hpp>


namespace Nil {
namespace Resource {


// ------------------------------------------------------------- [ Resource ] --
/*
  Font Resource
*/
struct Font
{
  const char        *name;
  enum { FILENAME } data_type;
  uintptr_t         data;
  size_t            data_size;

  // -- Output -- //
  
  Load_status       status;
  
  uint32_t *glyph_data;
  size_t    glyph_data_count;
  
  uint32_t      texture_id;
  const char    *typeface_name;
  uint32_t      glyph_count;
  uintptr_t     platform_resource;
  uint32_t      id;
};


// ------------------------------------------------------------- [ Get Data ] --


bool
find_by_name(const char *name, Font &out);


void
get(size_t *count, Font **out);


// ----------------------------------------------------------------- [ Load ] --


bool
load(Font &in);


// ----------------------------------------------------------------- [ Info ] --
/*
  Various information about Font data.
*/


const char *
get_type_name(const Font &in);


size_t
font_count();


} // ns
} // ns


#endif // inc guard
