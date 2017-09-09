#ifndef LIGHT_INCLUDED_25EFE5DF_D8EC_4D8B_913B_2AAF362CD586
#define LIGHT_INCLUDED_25EFE5DF_D8EC_4D8B_913B_2AAF362CD586


#include <nil/fwd.hpp>


namespace Nil {
namespace Data {


// ----------------------------------------------------------------- [ Data ] --


struct Light
{
  enum { POINT, DIR, SPOT } type;
  uint8_t color[3];
  
  float atten_const;
  float atten_linear;
  float atten_exponential;
  
  // -- Output -- //
  
  float position[3];
  float direction[3];
};


// -------------------------------------------------------------- [ Get/Set ] --


void
get(size_t *count, Light **out);


void
get(const Node &node, Light &out);


void
set(Node &node, const Light &in);


void
remove_light(Node &node);


// ----------------------------------------------------------------- [ Info ] --


bool
has_light(const Node &node);


bool
has(const Node &node, const Light &data);


uint64_t
get_type_id(const Light &in);


const char*
get_type_name(const Light &in);


size_t
light_count();


// ---------------------------------------------------------- [ Data Events ] --


void
events(const uint32_t data, size_t *count, Light **out_data, Node **out_node);


} // ns
} // ns


#endif // inc guard
