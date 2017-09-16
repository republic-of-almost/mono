#include <catch/catch.hpp>
#include <math/transform/transform.hpp>
#include <math/mat/mat4.hpp>

TEST_CASE("Transform")
{
  // --------------------------------------------------------------- [ Init ] --
  
  SECTION("init")
  {
    const math::transform trans = math::transform_init();
    
    REQUIRE(math::vec3_is_near(trans.position, math::vec3_zero()));
    REQUIRE(math::vec3_is_near(trans.scale, math::vec3_one()));
    REQUIRE(math::quat_is_near(trans.rotation, math::quat_init()));
  }
  
  SECTION("int with params")
  {
    const math::vec3 pos = math::vec3_init(0.1f);
    const math::vec3 scale = math::vec3_init(4.f);
    const math::quat rot = math::quat_init_with_axis_angle(0, 1, 0, math::tau());
    
    const math::transform trans = math::transform_init(pos, scale, rot);
    
    REQUIRE(math::vec3_is_near(trans.position, pos));
    REQUIRE(math::vec3_is_near(trans.scale, scale));
    REQUIRE(math::quat_is_near(trans.rotation, rot));
  }
  
  SECTION("init from world")
  {
    /*
      Rotation is broken.
    */
//    const math::vec3 pos_v = math::vec3_init(0.1f);
//    const math::vec3 scale_v = math::vec3_init(4.f);
//    const math::quat rot_q = math::quat_init_with_axis_angle(0, 1, 0, math::tau());
//  
//    const math::mat4 pos = math::mat4_translate(pos_v);
//    const math::mat4 scale = math::mat4_scale(scale_v);
//    const math::mat4 rot = math::mat4_rotate_around_axis(math::vec3_init(0,1,0), 0.1);
//    
//    const math::mat4 world = math::mat4_multiply(rot, scale, pos);
//    
//    const math::transform trans = math::transform_init_from_world_matrix(world);
//    
//    REQUIRE(math::vec3_is_near(trans.position, pos_v, 0.0001f));
//    REQUIRE(math::vec3_is_near(trans.scale, scale_v, 0.0001f));
//    REQUIRE(math::quat_is_near(trans.rotation, rot_q, 0.0001f));
  }
  
  // --------------------------------------------------------- [ Operations ] --
  
  SECTION("world mat")
  {
  }
  
  SECTION("lookat mat")
  {
  }
  
  SECTION("inherited")
  {
  }
  
  // -------------------------------------------------- [ Direction Vectors ] --
  
  SECTION("fwd")
  {
  }
  
  SECTION("up")
  {
  }
  
  SECTION("left")
  {
  }
}
