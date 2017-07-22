#ifndef RENDERABLE_INCLUDED_75C9D93A_7575_4AE3_A4F5_91D82D1C1DD9
#define RENDERABLE_INCLUDED_75C9D93A_7575_4AE3_A4F5_91D82D1C1DD9


#include <nil/fwd.hpp>
#include <stdint.h>
#include <stddef.h>


namespace Nil {
namespace Data {


// ----------------------------------------------------------------- [ Data ] --


struct Renderable
{
  uint32_t mesh_id;
  uint32_t material_id;
  
  // -- Output -- //
  
  float     world_mat[16];
};


// -------------------------------------------------------------- [ Get/Set ] --


void
get(size_t *count, Renderable **renderables);


void
get(const Node &node, Renderable &out);


void
set(Node &node, const Renderable &in);


void
remove_renderable(Node &node);


// ----------------------------------------------------------------- [ Info ] --


bool
has_renderable(const Node &node);


bool
has(const Node &node, const Renderable &data);


uint64_t
get_type_id(const Renderable &in);


const char*
get_type_name(const Renderable &in);


size_t
renderable_count();


// ---------------------------------------------------------- [ Data Events ] --


void
events(const uint32_t data, size_t *count, Renderable **out_data, Node **out_node);


} // ns
} // ns


#endif // inc guard
