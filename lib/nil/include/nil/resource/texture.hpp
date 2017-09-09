#ifndef TEXTURE_INCLUDED_56366719_6675_4A06_AEA8_5615D7AA8B07
#define TEXTURE_INCLUDED_56366719_6675_4A06_AEA8_5615D7AA8B07


#include <nil/fwd.hpp>


namespace Nil {
namespace Resource {


// ------------------------------------------------------------- [ Resource ] --


/*!
  The calling code should set the attributes.
  When you load a texture the id will be set.
  When the renderer loads the texture from file, the dimentions will be set.
  When the renderer loads the texture, the status will be set.
*/
struct Texture
{  
  // -- Input --//

  const char                      *name;      // Internally copied
  enum { FILENAME, DATA, LOCAL }  data_type;
  uintptr_t                 data;       // Internally copied
  size_t                    data_size;
  
  // -- Input / Output -- //
  
  uint32_t width;       // If data_type == DATA then this is input.
  uint32_t height;      // If data_type == DATA then this is input.
  uint32_t depth;       // If data_type == DATA then this is input.
  uint32_t components;  // If data_type == DATA then this is input.
  
  // -- Output -- //
  
  enum { PENDING, LOADED, FAILED, STATUS_COUNT, } status;
  uintptr_t                         platform_resource;
  uint32_t                          id;
};


// ----------------------------------------------------------------- [ Find ] --


/*!
  Searches for a Texture by name.
  if found returns true else returns false.
*/
bool
find_by_name(const char *name, Texture &out);


// ----------------------------------------------------------- [ Get / Load ] --


/*!
  Loads a new Texture.
  Does *not* update an existing texture if name already exists.
  If it fails to load it will return false.
*/
bool
load(Texture &in);


/*!
  Gets access to the underlying data.

  size_t count = 0;
  Texture *data = nullptr;
  get(&count, &data);
*/
void
get(size_t *count, Texture **in_out);


// ----------------------------------------------------------------- [ Info ] --


/*!
  Convence method good for templates and UI.
  returns the type name.
*/
const char *
get_type_name(const Texture &in);


} // ns
} // ns


#endif // inc guard
