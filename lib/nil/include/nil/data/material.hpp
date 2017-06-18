#ifndef MATERIAL_INCLUDED_75C9D93A_7575_4AE3_A4F5_91D82D1C1DD9
#define MATERIAL_INCLUDED_75C9D93A_7575_4AE3_A4F5_91D82D1C1DD9


#include <nil/fwd.hpp>
#include <stdint.h>
#include <stddef.h>


namespace Nil {
namespace Data {


// ----------------------------------------------------------------- [ Data ] --


struct Material
{
  enum { FULLBRIGHT, LIT, DIR_LIGHT }  shader;
  float     color[4];
  
  uint32_t  texture_01;
  uint32_t  texture_02;
  uint32_t  texture_03;
  
  float     scale[2];
  float     offset[2];
};


// -------------------------------------------------------------- [ Get/Set ] --


void
get(const Node &node, Material &out);


void
set(Node &node, const Material &in);


void
remove_window(Node &node);


// ----------------------------------------------------------------- [ Info ] --


bool
has_material(const Node &node);


uint64_t
get_type_id(const Material &in);


size_t
material_count();


// ---------------------------------------------------------- [ Data Events ] --


void
events(const uint32_t data, size_t *count, Material **out_data, Node **out_node);


} // ns
} // ns


#endif // inc guard
