#ifndef BOUDNING_BOX_INCLUDED_6CE5864B_07D2_411B_AB16_082F249FFC17
#define BOUDNING_BOX_INCLUDED_6CE5864B_07D2_411B_AB16_082F249FFC17


#include <nil/fwd.hpp>
#include <stdint.h>


namespace Nil {
namespace Data {


struct Bounding_box
{
  float min[3];
  float max[3];
};


void
get(const Node &node, Bounding_box &out);


void
set(Node &node, const Bounding_box &in);


bool
has_bounding_box(const Node &node);


uint64_t
get_type_id(const Bounding_box &in);


} // ns
} // ns


#endif // inc guard
