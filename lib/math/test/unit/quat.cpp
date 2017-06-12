#include <catch/catch.hpp>
#include <math/quat/quat.hpp>
#include <array>


TEST_CASE("Quat rotations")
{

  SECTION("Initialize")
  {
    const math::quat empty = math::quat_init();
    REQUIRE(math::get_x(empty) == 0.f);
    REQUIRE(math::get_y(empty) == 0.f);
    REQUIRE(math::get_z(empty) == 0.f);
    REQUIRE(math::get_w(empty) == 1.f);
    
    const math::quat one_two_three_four = math::quat_init(10, 0, 0, 0);
//    REQUIRE(math::get_x(one_two_three_four) == 1.f);
    REQUIRE(math::get_y(one_two_three_four) == 0.f);
    REQUIRE(math::get_z(one_two_three_four) == 0.f);
    REQUIRE(math::get_w(one_two_three_four) == 0.f);
  }
  
  SECTION("Angle axis")
  {
  }
  
  SECTION("Rotation Mat")
  {
    // Pitch
    {
      const math::quat pitch_rot = math::quat_init_with_axis_angle(1, 0, 0, math::tau());
      
      const math::mat3 mat_from_quat = math::quat_get_rotation_matrix(pitch_rot);
      const math::mat3 mat_data = math::mat3_rotation_pitch_from_euler(math::tau());
    
      REQUIRE(math::mat3_equal(mat_from_quat, mat_data));
    }
    
    // Roll
    {
      const math::quat roll_rot = math::quat_init_with_axis_angle(0, 0, 1, math::tau());
      
      const math::mat3 mat_from_quat = math::quat_get_rotation_matrix(roll_rot);
      const math::mat3 mat_data = math::mat3_rotation_roll_from_euler(math::tau());
    
      REQUIRE(math::mat3_equal(mat_from_quat, mat_data));
    }

    // Yaw
    {
      const math::quat yaw_rot = math::quat_init_with_axis_angle(0, 1, 0, math::tau());
      
      const math::mat3 mat_from_quat = math::quat_get_rotation_matrix(yaw_rot);
      const math::mat3 mat_data = math::mat3_rotation_yaw_from_euler(math::tau());
    
      REQUIRE(math::mat3_equal(mat_from_quat, mat_data));
    }
  }
  
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
