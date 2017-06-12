#ifndef MESH_INCLUDED_C2A6D08B_D0E1_4C9C_A85E_33CE15255EEF
#define MESH_INCLUDED_C2A6D08B_D0E1_4C9C_A85E_33CE15255EEF


#include <nil/fwd.hpp>
#include <stdint.h>


namespace Nil {
namespace Data {


struct Mesh
{
  uint32_t mesh_id;
  uint32_t index_id;
  
  uint32_t start;
  uint32_t count;
};


void
get(const Node &node, Mesh &out);


void
set(Node &node, const Mesh &in);


bool
has_mesh(const Node &node);


uint64_t
get_type_id(const Mesh &in);


} // ns
} // ns


#endif // inc guard
