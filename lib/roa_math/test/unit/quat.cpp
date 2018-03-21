#include <catch/catch.hpp>
#include <roa_math/math.h>
#include <array>


TEST_CASE("Quat rotations")
{
	
  SECTION("Initialize")
  {
    roa_quaternion empty = roa_quaternion_default();
    REQUIRE(roa_quaternion_get_x(empty) == 0.f);
    REQUIRE(roa_quaternion_get_y(empty) == 0.f);
    REQUIRE(roa_quaternion_get_z(empty) == 0.f);
    REQUIRE(roa_quaternion_get_w(empty) == 1.f);
    
    roa_quaternion norm = roa_quaternion_set_with_values(10.f, 0.f, 0.f, 0.f);
    REQUIRE(roa_quaternion_get_x(norm) == 1.f);
    REQUIRE(roa_quaternion_get_y(norm) == 0.f);
    REQUIRE(roa_quaternion_get_z(norm) == 0.f);
    REQUIRE(roa_quaternion_get_w(norm) == 0.f);
  }
	
  
  SECTION("Angle axis")
  {
		/* not test data */
  }
  
	/*
  SECTION("Rotation Mat")
  {
    // Pitch
    {
      const math::quat pitch_rot = math::quat_init_with_axis_angle(1, 0, 0, math::tau());
      
      const math::mat3 mat_from_quat = math::quat_get_rotation_matrix(pitch_rot);
      const math::mat3 mat_data = math::mat3_rotation_pitch_from_euler(math::tau());
    
      REQUIRE(math::mat3_is_near(mat_from_quat, mat_data));
    }
    
    // Roll
    {
      const math::quat roll_rot = math::quat_init_with_axis_angle(0, 0, 1, math::tau());
      
      const math::mat3 mat_from_quat = math::quat_get_rotation_matrix(roll_rot);
      const math::mat3 mat_data = math::mat3_rotation_roll_from_euler(math::tau());
    
      REQUIRE(math::mat3_is_near(mat_from_quat, mat_data));
    }

    // Yaw
    {
      const math::quat yaw_rot = math::quat_init_with_axis_angle(0, 1, 0, math::tau());
      
      const math::mat3 mat_from_quat = math::quat_get_rotation_matrix(yaw_rot);
      const math::mat3 mat_data = math::mat3_rotation_yaw_from_euler(math::tau());
    
      REQUIRE(math::mat3_is_near(mat_from_quat, mat_data));
    }
  }
	*/
  
  SECTION("Euler")
  {
  }
  
  SECTION("Rotation")
  {
  }
  
  SECTION("Multiplication")
  {
  }
}
