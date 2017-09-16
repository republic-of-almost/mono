#ifndef TRANSFORM_INCLUDED_E79342EA_D69A_4FEB_BEAD_BA21D7B8B8E6
#define TRANSFORM_INCLUDED_E79342EA_D69A_4FEB_BEAD_BA21D7B8B8E6


#include "../detail/detail.hpp"
#include "../transform/transform_types.hpp"
#include "../quat/quat.hpp"
#include "../vec/vec3.hpp"
#include "../mat/mat4.hpp"


// --------------------------------------------------------------- [ Config ] --


#ifndef MATH_TRANS_INLINE
#define MATH_TRANS_INLINE inline
#endif


_MATH_NS_OPEN


// ----------------------------------------------------------------- [ Init ] --
/*
  Create a transform with some default data.
*/


MATH_TRANS_INLINE transform    transform_init();
MATH_TRANS_INLINE transform    transform_init(const vec3 position, const vec3 scale, const quat &rotation);
//MATH_TRANS_INLINE transform    transform_init_from_world_matrix(const mat4 &matrix); // Rotation is borked.


// ----------------------------------------------------------- [ Operations ] --
/*
  Convert a transform into other forms, or inherit it from another transform.
*/


MATH_TRANS_INLINE mat4         transform_world_matrix(const transform &transform);
MATH_TRANS_INLINE mat4         transform_lookat_matrix(const transform &to_view, const vec3 world_fwd, const vec3 world_up);
MATH_TRANS_INLINE transform    transform_inherited(const transform &parent, const transform &child);


// ---------------------------------------------------- [ Direction Vectors ] --
/*
  Change the world directions to suit, and the local transforms will use that
  to give you the relative direction.
*/


MATH_TRANS_INLINE vec3         transform_world_fwd();
MATH_TRANS_INLINE vec3         transform_world_up();
MATH_TRANS_INLINE vec3         transform_world_left();

MATH_TRANS_INLINE vec3         transform_fwd(const transform &trans);
MATH_TRANS_INLINE vec3         transform_up(const transform &trans);
MATH_TRANS_INLINE vec3         transform_left(const transform &trans);


// ------------------------------------------------------------ [ Init Impl ] --


transform
transform_init()
{
  return {
    MATH_NS_NAME::vec3_zero(),
    MATH_NS_NAME::vec3_one(),
    MATH_NS_NAME::quat_init()
  };
}


transform
transform_init(const vec3 position, const vec3 scale, const quat &rotation)
{
  MATH_NS_NAME::transform return_transform;
  return_transform.position = position;
  return_transform.scale    = scale;
  return_transform.rotation = rotation;

  return return_transform;
}


//transform
//transform_init_from_world_matrix(const mat4 &matrix)
//{
//  // get position
//  const float x = MATH_NS_NAME::mat4_get(matrix, 3, 0);
//  const float y = MATH_NS_NAME::mat4_get(matrix, 3, 1);
//  const float z = MATH_NS_NAME::mat4_get(matrix, 3, 2);
//
//  const MATH_NS_NAME::vec3 position = MATH_NS_NAME::vec3_init(x, y, z);
//
//  // get scale.
//  const float s_x = MATH_NS_NAME::sign(mat4_get(matrix, 0, 0)) *
//                    MATH_NS_NAME::sqrt((mat4_get(matrix, 0, 0) * mat4_get(matrix, 0, 0)) +
//                               (mat4_get(matrix, 0, 1) * mat4_get(matrix, 0, 1)) +
//                               (mat4_get(matrix, 0, 2) * mat4_get(matrix, 0, 2)));
//
//  const float s_y = MATH_NS_NAME::sign(mat4_get(matrix, 1, 1)) *
//                    MATH_NS_NAME::sqrt((mat4_get(matrix, 1, 0) * mat4_get(matrix, 1, 0)) +
//                               (mat4_get(matrix, 1, 1) * mat4_get(matrix, 1, 1)) +
//                               (mat4_get(matrix, 1, 2) * mat4_get(matrix, 1, 2)));
//
//  const float s_z = MATH_NS_NAME::sign(mat4_get(matrix, 2, 2)) *
//                    MATH_NS_NAME::sqrt((mat4_get(matrix, 2, 0) * mat4_get(matrix, 2, 0)) +
//                               (mat4_get(matrix, 2, 1) * mat4_get(matrix, 2, 1)) +
//                               (mat4_get(matrix, 2, 2) * mat4_get(matrix, 2, 2)));
//
//  const vec3 scale = vec3_init(s_x, s_y, s_z);
//
//  // get rotation.
//  const mat3 sub_mat  = mat4_get_sub_mat3(matrix);
//  const quat rotation = quat_init_with_mat3(sub_mat);
//
//  return transform_init(position, scale, rotation);
//}


// ------------------------------------------------------ [ Operations Impl ] --


transform
transform_inherited(const transform &parent, const transform &child)
{
  transform inherited;

  inherited.scale    = MATH_NS_NAME::vec3_multiply(parent.scale, child.scale);
  inherited.rotation = MATH_NS_NAME::quat_multiply(parent.rotation, child.rotation);
  
  inherited.position = MATH_NS_NAME::vec3_add(
    parent.position,
    MATH_NS_NAME::quat_rotate_point(
      parent.rotation,
        MATH_NS_NAME::vec3_multiply(
          child.position,
          parent.scale
        )
      )
    );

  return inherited;
}


mat4
transform_world_matrix(const transform &to_world)
{
  // Get scale
  const mat4 scale = mat4_scale(to_world.scale);

  // Get rotation
  mat4 rotation = mat4_init(quat_get_rotation_matrix(to_world.rotation));
  rotation.data[15] = 1.f;

  // Get translation
  const mat4 translation = mat4_translate(to_world.position);

  return mat4_multiply(scale, rotation, translation);
}


mat4
transform_lookat_matrix(const transform &to_view, const vec3 world_fwd, const vec3 world_up)
{
  const math::vec3 cam_fwd  = math::quat_rotate_point(to_view.rotation, world_fwd);
  const math::vec3 look_fwd = math::vec3_add(to_view.position, cam_fwd);
  const math::vec3 look_up  = math::quat_rotate_point(to_view.rotation, world_up);

  return math::mat4_lookat(to_view.position, look_fwd, look_up);
}


// ----------------------------------------------- [ Direction Vectors Impl ] --


vec3
transform_world_fwd()
{
  return math::vec3_init(0,0,1);
}


vec3
transform_world_up()
{
  return math::vec3_init(0,1,0);
}


vec3
transform_world_left()
{
  return math::vec3_init(1,0,0);
}


vec3
transform_fwd(const transform &trans)
{
  const math::vec3 rot_dir = math::quat_rotate_point(
    trans.rotation,
    transform_world_fwd()
  );
  
  return math::vec3_normalize(rot_dir);
}


vec3
transform_up(const transform &trans)
{
  const math::vec3 rot_dir = math::quat_rotate_point(
    trans.rotation,
    transform_world_up()
  );
  
  return math::vec3_normalize(rot_dir);
}


vec3
transform_left(const transform &trans)
{
  const math::vec3 rot_dir = math::quat_rotate_point(
    trans.rotation,
    transform_world_left()
  );
  
  return math::vec3_normalize(rot_dir);
}


_MATH_NS_CLOSE


#endif // include guard
