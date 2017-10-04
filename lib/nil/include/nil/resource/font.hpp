#ifndef FONT_INCLUDED_FAE30313_D649_4821_B300_7C4AA5FF4885
#define FONT_INCLUDED_FAE30313_D649_4821_B300_7C4AA5FF4885


#include <nil/fwd.hpp>


struct Nil_font
{
  const char        *name;
  enum { FILENAME } data_type;
  uintptr_t         data;
  size_t            data_size;

  // -- Output -- //
  
  Nil_resource_status       status;
  
  uint32_t *glyph_data;
  size_t    glyph_data_count;
  
  uint32_t      texture_id;
  const char    *typeface_name;
  uint32_t      glyph_count;
  uintptr_t     platform_resource;
  uint32_t      id;
};




#endif // inc guard
