#include <catch/catch.hpp>

TEST_CASE("Transform")
{
  // --------------------------------------------------------------- [ Init ] --
  
  /*
	SECTION("init")
  {
    const math::transform trans = math::transform_init();
    
    REQUIRE(math::vec3_is_near(trans.position, math::vec3_zero()));
    REQUIRE(math::vec3_is_near(trans.scale, math::vec3_one()));
    REQUIRE(math::quat_is_near(trans.rotation, math::quat_init()));
  }
	*/
  
	/*
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
	*/
  
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
    // mat4 currently doens't support matrix decomposition.
  }
  
  SECTION("lookat mat")
  {
    // need data
  }
  
	/*
  SECTION("inherited")
  {
    // Rotation is required
    
    const math::vec3 pos = math::vec3_init(0.5f);
    const math::vec3 scale = math::vec3_init(0.5f);
    
    const math::transform trans_a = math::transform_init(pos, scale, math::quat_init());
    const math::transform inherited = math::transform_inherited(trans_a, trans_a);
    
    const math::vec3 expected_pos = math::vec3_init(0.75f);
    const math::vec3 expected_scale = math::vec3_init(0.25f);
    
    REQUIRE(math::vec3_is_near(inherited.position, expected_pos));
    REQUIRE(math::vec3_is_near(inherited.scale, expected_scale));
  }
	*/
  
  // -------------------------------------------------- [ Direction Vectors ] --
  
	/*
  SECTION("fwd")
  {
    const math::vec3 w_dir = math::transform_world_fwd();
    const math::transform trans = math::transform_init();
    const math::vec3 l_dir = math::transform_fwd(trans);
    
    REQUIRE(math::vec3_is_near(w_dir, l_dir, 0.00001));
  }
	*/
  
	/*
  SECTION("up")
  {
    const math::vec3 w_dir = math::transform_world_up();
    const math::transform trans = math::transform_init();
    const math::vec3 l_dir = math::transform_up(trans);
    
    REQUIRE(math::vec3_is_near(w_dir, l_dir, 0.00001));
  }*/
  
	/*
  SECTION("left")
  {
    const math::vec3 w_dir = math::transform_world_left();
    const math::transform trans = math::transform_init();
    const math::vec3 l_dir = math::transform_left(trans);
    
		REQUIRE(math::vec3_is_near(w_dir, l_dir, 0.00001));
  }
	*/
}
