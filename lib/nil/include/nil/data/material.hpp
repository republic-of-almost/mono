#ifndef MATERIAL_INCLUDED_75C9D93A_7575_4AE3_A4F5_91D82D1C1DD9
#define MATERIAL_INCLUDED_75C9D93A_7575_4AE3_A4F5_91D82D1C1DD9


#include <nil/fwd.hpp>
#include <stdint.h>


namespace Nil {
namespace Data {


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


void
get(const Node &node, Material &out);


void
set(Node &node, const Material &in);


bool
has_material(const Node &node);


uint64_t
get_type_id(const Material &in);


} // ns
} // ns


#endif // inc guard
