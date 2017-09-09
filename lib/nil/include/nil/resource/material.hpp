#ifndef MATERIAL_INCLUDED_F58F218E_C94E_49D4_9CA3_34CBD7A35E26
#define MATERIAL_INCLUDED_F58F218E_C94E_49D4_9CA3_34CBD7A35E26


#include <nil/fwd.hpp>


namespace Nil {
namespace Resource {


// ------------------------------------------------------------- [ Resource ] --


/*!
  The calling code should set the attributes.
  When you successfully load a material the id field is updated.
*/
struct Material
{
  // -- Input -- //

  const char *name;     // Internally copied
  uint32_t color;
  uint32_t texture_01;
  uint32_t texture_02;
  uint32_t texture_03;
  
  // -- Output -- //
  
  uint32_t id;
};


// ----------------------------------------------------------------- [ Find ] --


/*!
  Searches for a Material by name.
  if found returns true else returns false.
*/
bool
find_by_name(const char *name, Material &out);


// ------------------------------------------------------------ [ Set / Get ] --


/*!
  Loads a new Material.
  Updates an existing material if name exists.
  If it fails to load it will return false.
*/
bool
load(Material &in_out);


/*!
  Gets access to the underlying data.

  size_t count = 0;
  Matieral *data = nullptr;
  get(&count, &data);
*/
void
get(size_t *count, Material **out);


// ----------------------------------------------------------------- [ Info ] --


/*!
  Convence method good for templates and UI.
  returns the type name.
*/
const char *
get_type_name(const Material &in);


} // ns
} // ns


#endif // inc guard
