#include <catch/catch.hpp>

#define MATH_USE_SIMD

#include <math/vec/vec3.hpp>
#include <array>


TEST_CASE("Vector 3 SIMD")
{
  REQUIRE(MATH_ON_SIMD);
  REQUIRE(MATH_ON_SSE2);

  // Initialize test data that we will use
  const math::vec3 one_two_three = math::vec3_init(1.f, 2.f, 3.f);
  REQUIRE(math::vec3_get_x(one_two_three) == 1.f);
  REQUIRE(math::vec3_get_y(one_two_three) == 2.f);
  REQUIRE(math::vec3_get_z(one_two_three) == 3.f);

  const math::vec3 four = math::vec3_init(4.f);
  REQUIRE(math::vec3_get_x(four) == 4.f);
  REQUIRE(math::vec3_get_y(four) == 4.f);
  REQUIRE(math::vec3_get_z(four) == 4.f);

  const std::array<float, 3> arr_data = {5.f, 6.f, 7.f};
  const math::vec3 five_six_seven = math::vec3_init_with_array(&arr_data[0]);
  REQUIRE(math::vec3_get_x(five_six_seven) == 5.f);
  REQUIRE(math::vec3_get_y(five_six_seven) == 6.f);
  REQUIRE(math::vec3_get_z(five_six_seven) == 7.f);
  
  const float error = 0.0001f; // Error for near tests


  SECTION("Export Data")
  {
    std::array<float, 3> arr_export = {0.f, 0.f, 0.f};
    const math::vec3 to_export = math::vec3_one();
    math::vec3_to_array(to_export, arr_export.data());
    
    REQUIRE(arr_export.at(0) == 1.f);
    REQUIRE(arr_export.at(1) == 1.f);
    REQUIRE(arr_export.at(2) == 1.f);
  }
  

	SECTION("Constants")
	{
		const math::vec3 zero = math::vec3_zero();
    REQUIRE(math::vec3_get_x(zero) == 0.f);
    REQUIRE(math::vec3_get_y(zero) == 0.f);
    REQUIRE(math::vec3_get_z(zero) == 0.f);
    
    const math::vec3 one = math::vec3_one();
    REQUIRE(math::vec3_get_x(one) == 1.f);
    REQUIRE(math::vec3_get_y(one) == 1.f);
    REQUIRE(math::vec3_get_z(one) == 1.f);
    
    const math::vec3 zero_zero_one = math::vec3_zero_zero_one();
    REQUIRE(math::vec3_get_x(zero_zero_one) == 0.f);
    REQUIRE(math::vec3_get_y(zero_zero_one) == 0.f);
    REQUIRE(math::vec3_get_z(zero_zero_one) == 1.f);
	}
  
  
  SECTION("Equality")
  {
    REQUIRE(math::vec3_is_equal(math::vec3_one(), math::vec3_one()));
    REQUIRE(math::vec3_is_not_equal(math::vec3_one(), math::vec3_zero()));
    
    REQUIRE(math::vec3_is_near(math::vec3_zero(), math::vec3_one(), 1.f));
    REQUIRE(math::vec3_is_not_near(math::vec3_zero(), math::vec3_one(), 0.5f));
  }
  
  
  SECTION("Basic Arithmetic")
  {
    const math::vec3 addition = math::vec3_add(one_two_three, four);
    REQUIRE(math::vec3_is_near(addition, five_six_seven, error));
    
    const math::vec3 subtraction = math::vec3_subtract(addition, four);
    REQUIRE(math::vec3_is_near(subtraction, one_two_three, error));
    
    const math::vec3 mulitplication = math::vec3_multiply(one_two_three, five_six_seven);
    const math::vec3 expected_multi = math::vec3_init(5.f, 12.f, 21.f);
    REQUIRE(math::vec3_is_near(expected_multi, mulitplication, error));
    
    const math::vec3 division = math::vec3_divide(expected_multi, five_six_seven);
    REQUIRE(math::vec3_is_near(division, one_two_three, error));
  }
  
  
  SECTION("Scale")
  {
    const math::vec3 scale_a = math::vec3_scale(one_two_three, 3.f);
    const math::vec3 expected_a = math::vec3_init(3.f, 6.f, 9.f);
    REQUIRE(math::vec3_is_near(scale_a, expected_a, error));
    
    const math::vec3 scale_b = math::vec3_scale(one_two_three, 0.5f);
    const math::vec3 expected_b = math::vec3_init(0.5f, 1.f, 1.5f);
    REQUIRE(math::vec3_is_near(scale_b, expected_b, error));
  }
  
  
  SECTION("Lerp")
  {
    const math::vec3 lerp_start = math::vec3_zero();
    const math::vec3 lerp_end   = math::vec3_one();

    const math::vec3 dt_0 = math::vec3_lerp(lerp_start, lerp_end, 0.f);
    REQUIRE(math::vec3_is_near(dt_0, math::vec3_zero(), error));

    const math::vec3 dt_025 = math::vec3_lerp(lerp_start, lerp_end, 0.25f);
    REQUIRE(math::vec3_is_near(dt_025, math::vec3_init(0.25f, 0.25f, 0.25f), error));

    const math::vec3 dt_05 = math::vec3_lerp(lerp_start, lerp_end, 0.5f);
    REQUIRE(math::vec3_is_near(dt_05, math::vec3_init(0.5f, 0.5f, 0.5f), error));

    const math::vec3 dt_075 = math::vec3_lerp(lerp_start, lerp_end, 0.75f);
    REQUIRE(math::vec3_is_near(dt_075, math::vec3_init(0.75f, 0.75f, 0.75f), error));

    const math::vec3 dt_1 = math::vec3_lerp(lerp_start, lerp_end, 1.f);
    REQUIRE(math::vec3_is_near(dt_1, math::vec3_one(), error));
  }
  

  SECTION("Normalize")
  {
    const math::vec3 unnormilized_vec = math::vec3_init(1.1f, 2.2f, 3.3f);
    const math::vec3 normalize_vec = math::vec3_normalize(unnormilized_vec);
    REQUIRE(math::vec3_is_near(normalize_vec, math::vec3_init(0.2672f, 0.5345, 0.80178), error));
  }
  
  
  SECTION("Dot Product")
  {
    const math::vec3 dot_vec_a = math::vec3_init(1.1f, 2.2f, 3.3f);
    const math::vec3 dot_vec_b = math::vec3_init(1.1f, 2.2f, 2.2f);
    const float dot_prod = math::vec3_dot(dot_vec_a, dot_vec_b);
    REQUIRE(math::is_near(dot_prod, 13.31f, error));
  }
  
  
  SECTION("Length")
  {
    const math::vec3 one_two = math::vec3_init(1.1f, 2.2f, 3.3f);
    const float length_of_vec = math::vec3_length(one_two);
    REQUIRE(math::is_near(length_of_vec, 4.11582f, error));
  }
  
  
  SECTION("Cross")
  {
    const math::vec3 cross_vec_a = math::vec3_init(1.1f, 2.2f, 3.3f);
    const math::vec3 cross_vec_b = math::vec3_init(1.1f, 2.2f, 2.2f);
    const math::vec3 cross = math::vec3_cross(cross_vec_a, cross_vec_b);
    REQUIRE(math::vec3_is_near(cross, math::vec3_init(-2.42f, 1.21f, 0.f), error));

  }
}