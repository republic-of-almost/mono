#ifndef BOUDNING_BOX_INCLUDED_6CE5864B_07D2_411B_AB16_082F249FFC17
#define BOUDNING_BOX_INCLUDED_6CE5864B_07D2_411B_AB16_082F249FFC17


#include <nil/fwd.hpp>
#include <stdint.h>
#include <stddef.h>


namespace Nil {
namespace Data {


// ----------------------------------------------------------------- [ Data ] --


struct Bounding_box
{
  float min[3];
  float max[3];
};


// -------------------------------------------------------------- [ Get/Set ] --


void
get(const Node &node, Bounding_box &out, const bool world = false);


void
set(Node &node, const Bounding_box &in);


// ----------------------------------------------------------------- [ Info ] --


bool
has_bounding_box(const Node &node);


bool
has(const Node &node, const Bounding_box &data);


uint64_t
get_type_id(const Bounding_box &in);


const char*
get_type_name(const Bounding_box &in);


size_t
bounding_box_count();


// --------------------------------------------------------------- [ Events ] --


void
events(const uint32_t data, size_t *count, Window **out_data, Node **out_node);


} // ns
} // ns


#endif // inc guard
