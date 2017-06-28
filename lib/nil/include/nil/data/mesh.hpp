#ifndef MESH_INCLUDED_C2A6D08B_D0E1_4C9C_A85E_33CE15255EEF
#define MESH_INCLUDED_C2A6D08B_D0E1_4C9C_A85E_33CE15255EEF


#include <nil/fwd.hpp>
#include <stdint.h>
#include <stddef.h>


namespace Nil {
namespace Data {


// ----------------------------------------------------------------- [ Data ] --


struct Mesh
{
  uint32_t mesh_id;
  uint32_t index_id;
  
  uint32_t start;
  uint32_t count;
};


// -------------------------------------------------------------- [ Get/Set ] --


void
get(size_t *count, Mesh **data);


void
get(const Node &node, Mesh &out);


void
set(Node &node, const Mesh &in);


void
remove_window(Node &node);


// ----------------------------------------------------------------- [ Info ] --


bool
has_mesh(const Node &node);


bool
has(const Node &node, const Mesh &data);


uint64_t
get_type_id(const Mesh &in);


const char*
get_type_name(const Mesh &in);


size_t
mesh_count();


// ---------------------------------------------------------- [ Data Events ] --


void
events(const uint32_t data, size_t *count, Mesh **out_data, Node **out_node);


} // ns
} // ns


#endif // inc guard
