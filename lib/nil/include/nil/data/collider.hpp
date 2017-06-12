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


namespace Nil {
namespace Data {


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



void
get(const Node &node, Collider &out);


void
set(Node &node, const Collider &in);


bool
has_collider(const Node &node);


uint64_t
get_type_id(const Collider &in);



} // ns
} // ns


#endif // inc guard
