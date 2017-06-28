/*
  Colider
  --
  Collider data comes with a type id that you can extend to include other types.
  eg.
  namespace Collider_types {
  enum YOUR_ENUM {
    YOUR_TYPE = Collider_type::COUNT,
    YOUR_OTHER_TYPE,
  };
  }
  
  5 args should be enough to cover most collider types.
  eg.
  BoxCollider(extents_vec3);
  StaticPlane(normal_vec3, scalar);
*/
#ifndef COLLIDER_INCLUDED_F2023946_DF09_42BE_B3FC_A3B7E926D5AE
#define COLLIDER_INCLUDED_F2023946_DF09_42BE_B3FC_A3B7E926D5AE


#include <nil/fwd.hpp>
#include <stdint.h>
#include <stddef.h>


namespace Nil {
namespace Data {


// ----------------------------------------------------------------- [ Data ] --


namespace Collider_type {
enum ENUM {

  BOX,
  PLANE,
  SPHERE,
  CAPSULE,
  
  COUNT
};
}


struct Collider
{
  uint32_t  type;
  
  float     arg_01;
  float     arg_02;
  float     arg_03;
  float     arg_04;
  float     arg_05;
};


// -------------------------------------------------------------- [ Get/Set ] --


void
get(size_t *count, Collider **data);


void
get(const Node &node, Collider &out);


void
set(Node &node, const Collider &in);


void
remove_collider(Node &node);


// ----------------------------------------------------------------- [ Info ] --


bool
has_collider(const Node &node);


bool
has(const Node &node, const Collider &data);


uint64_t
get_type_id(const Collider &in);


const char*
get_type_name(const Collider &in);


size_t
collider_count();


// ---------------------------------------------------------- [ Data Events ] --


void
events(const uint32_t data, size_t *count, Collider **out_data, Node **out_node);




} // ns
} // ns


#endif // inc guard
