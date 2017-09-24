#ifndef TRANSFORM_INCLUDED_FDDE6A65_DD29_4846_B08C_7B64E56519C2
#define TRANSFORM_INCLUDED_FDDE6A65_DD29_4846_B08C_7B64E56519C2


#include <roa/fundamental.hpp>
#include <roa/object.hpp>


namespace ROA {


class Transform final : public Object
{
public:


  // ----------------------------------------------------------- [ Lifetime ] --


  explicit      Transform();
  explicit      Transform(const ROA_nullptr null);
  explicit      Transform(const Vector3 &pos,
                          const Vector3 &scale,
                          const Quaternion &rotation);
  
  
  // --------------------------------------------------------- [ Attributes ] --
  
  
  Vector3       get_position() const;
  Vector3       get_world_position() const;
  void          set_position(const Vector3 &in);
  
  Vector3       get_scale() const;
  Vector3       get_world_scale() const;
  void          set_scale(const Vector3 &in);
  
  Quaternion    get_rotation() const;
  Quaternion    get_world_rotation() const;
  void          set_rotation(const Quaternion &in);
  
  Vector3       get_forward() const;
  Vector3       get_world_forward() const;
  
  Vector3       get_up() const;
  Vector3       get_world_up() const;
  
  Vector3       get_left() const;
  Vector3       get_world_left() const;

};


} // ns


#endif // inc guard
