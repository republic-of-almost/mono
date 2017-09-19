#ifndef RENDERABLE_TEXT_INCLUDED_621C658B_0D6F_4DD3_A06A_4733302FF9B8
#define RENDERABLE_TEXT_INCLUDED_621C658B_0D6F_4DD3_A06A_4733302FF9B8


#include <nil/fwd.hpp>


namespace Nil {
namespace Data {


// ----------------------------------------------------------------- [ Data ] --


struct Renderable_text
{
  uint32_t      font_id;
  const char    *text;
  
  // -- Output -- //
  
  float         world_mat[16];

};


// -------------------------------------------------------------- [ Get/Set ] --


void
get(size_t *count, Renderable_text **renderables);


void
get(const Node &node, Renderable_text &out);


void
set(Node &node, const Renderable_text &in);


void
remove_renderable_text(Node &node);



// ----------------------------------------------------------------- [ Info ] --


bool
has_renderable_text(const Node &node);


bool
has(const Node &node, const Renderable_text &data);


uint64_t
get_type_id(const Renderable_text &in);


const char*
get_type_name(const Renderable_text &in);


size_t
renderable_text_count();



// ---------------------------------------------------------- [ Data Events ] --


void
events(const uint32_t data, size_t *count, Renderable_text **out_data, Node **out_node);


} // ns
} // ns


#endif // inc guard
