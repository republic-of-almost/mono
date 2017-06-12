#include <catch/catch.hpp>

#define MATH_USE_SIMD

#include <math/vec/vec4.hpp>
#include <array>


TEST_CASE("Vector 4 SIMD")
{
  REQUIRE(MATH_ON_SIMD);
  REQUIRE(MATH_ON_SSE2);

  // Initialize test data that we will use
  const math::vec4 one_two_three_four = math::vec4_init(1.f, 2.f, 3.f, 4.f);
  REQUIRE(math::vec4_get_x(one_two_three_four) == 1.f);
  REQUIRE(math::vec4_get_y(one_two_three_four) == 2.f);
  REQUIRE(math::vec4_get_z(one_two_three_four) == 3.f);
  REQUIRE(math::vec4_get_w(one_two_three_four) == 4.f);

  const math::vec4 four = math::vec4_init(4.f);
  REQUIRE(math::vec4_get_x(four) == 4.f);
  REQUIRE(math::vec4_get_y(four) == 4.f);
  REQUIRE(math::vec4_get_z(four) == 4.f);
  REQUIRE(math::vec4_get_w(four) == 4.f);

  const std::array<float, 4> arr_data = {5.f, 6.f, 7.f, 8.f};
  const math::vec4 five_six_seven_eight = math::vec4_init_with_array(&arr_data[0]);
  REQUIRE(math::vec4_get_x(five_six_seven_eight) == 5.f);
  REQUIRE(math::vec4_get_y(five_six_seven_eight) == 6.f);
  REQUIRE(math::vec4_get_z(five_six_seven_eight) == 7.f);
  REQUIRE(math::vec4_get_w(five_six_seven_eight) == 8.f);
  
  const float error = 0.0001f; // Error for near tests


  SECTION("Export Data")
  {
    std::array<float, 4> arr_export = {0.f, 0.f, 0.f, 0.f};
    const math::vec4 to_export = math::vec4_one();
    math::vec4_to_array(to_export, arr_export.data());
    
    REQUIRE(arr_export.at(0) == 1.f);
    REQUIRE(arr_export.at(1) == 1.f);
    REQUIRE(arr_export.at(2) == 1.f);
    REQUIRE(arr_export.at(3) == 1.f);
  }
  

	SECTION("Constants")
	{
		const math::vec4 zero = math::vec4_zero();
    REQUIRE(math::vec4_get_x(zero) == 0.f);
    REQUIRE(math::vec4_get_y(zero) == 0.f);
    REQUIRE(math::vec4_get_z(zero) == 0.f);
    REQUIRE(math::vec4_get_w(zero) == 0.f);
    
    const math::vec4 one = math::vec4_one();
    REQUIRE(math::vec4_get_x(one) == 1.f);
    REQUIRE(math::vec4_get_y(one) == 1.f);
    REQUIRE(math::vec4_get_z(one) == 1.f);
    REQUIRE(math::vec4_get_w(one) == 1.f);
    
    const math::vec4 zero_zero_zero_one = math::vec4_zero_zero_zero_one();
    REQUIRE(math::vec4_get_x(zero_zero_zero_one) == 0.f);
    REQUIRE(math::vec4_get_y(zero_zero_zero_one) == 0.f);
    REQUIRE(math::vec4_get_z(zero_zero_zero_one) == 0.f);
    REQUIRE(math::vec4_get_w(zero_zero_zero_one) == 1.f);

	}
  
  
  SECTION("Equality")
  {
    REQUIRE(math::vec4_is_equal(math::vec4_one(), math::vec4_one()));
    REQUIRE(math::vec4_is_not_equal(math::vec4_one(), math::vec4_zero()));
    
    REQUIRE(math::vec4_is_near(math::vec4_zero(), math::vec4_one(), 1.f));
    REQUIRE(math::vec4_is_not_near(math::vec4_zero(), math::vec4_one(), 0.5f));
  }
  
  
  SECTION("Basic Arithmetic")
  {
    const math::vec4 addition = math::vec4_add(one_two_three_four, four);
    REQUIRE(math::vec4_is_near(addition, five_six_seven_eight, error));
    
    const math::vec4 subtraction = math::vec4_subtract(addition, four);
    REQUIRE(math::vec4_is_near(subtraction, one_two_three_four, error));
    
    const math::vec4 mulitplication = math::vec4_multiply(one_two_three_four, five_six_seven_eight);
    const math::vec4 expected_multi = math::vec4_init(5.f, 12.f, 21.f, 32.f);
    REQUIRE(math::vec4_is_near(expected_multi, mulitplication, error));
    
    const math::vec4 division = math::vec4_divide(expected_multi, five_six_seven_eight);
    REQUIRE(math::vec4_is_near(division, one_two_three_four, error));
  }
  
  
  SECTION("Scale")
  {
    const math::vec4 scale_a = math::vec4_scale(one_two_three_four, 3.f);
    const math::vec4 expected_a = math::vec4_init(3.f, 6.f, 9.f, 12.f);
    REQUIRE(math::vec4_is_near(scale_a, expected_a, error));
    
    const math::vec4 scale_b = math::vec4_scale(one_two_three_four, 0.5f);
    const math::vec4 expected_b = math::vec4_init(0.5f, 1.f, 1.5f, 2.f);
    REQUIRE(math::vec4_is_near(scale_b, expected_b, error));
  }
  
  
  SECTION("Lerp")
  {
    const math::vec4 lerp_start = math::vec4_zero();
    const math::vec4 lerp_end   = math::vec4_one();

    const math::vec4 dt_0 = math::vec4_lerp(lerp_start, lerp_end, 0.f);
    REQUIRE(math::vec4_is_near(dt_0, math::vec4_zero(), error));

    const math::vec4 dt_025 = math::vec4_lerp(lerp_start, lerp_end, 0.25f);
    REQUIRE(math::vec4_is_near(dt_025, math::vec4_init(0.25f, 0.25f, 0.25f, 0.25), error));

    const math::vec4 dt_05 = math::vec4_lerp(lerp_start, lerp_end, 0.5f);
    REQUIRE(math::vec4_is_near(dt_05, math::vec4_init(0.5f, 0.5f, 0.5f, 0.5f), error));

    const math::vec4 dt_075 = math::vec4_lerp(lerp_start, lerp_end, 0.75f);
    REQUIRE(math::vec4_is_near(dt_075, math::vec4_init(0.75f, 0.75f, 0.75f, 0.75f), error));

    const math::vec4 dt_1 = math::vec4_lerp(lerp_start, lerp_end, 1.f);
    REQUIRE(math::vec4_is_near(dt_1, math::vec4_one(), error));
  }
  

  SECTION("Normalize")
  {
    const math::vec4 unnormilized_vec = math::vec4_init(1.1f, 2.2f, 3.3f, 4.4f);
    const math::vec4 normalize_vec = math::vec4_normalize(unnormilized_vec);
    REQUIRE(math::vec4_is_near(normalize_vec, math::vec4_init(0.182574f, 0.365148f, 0.547723f, 0.730297f), error));
  }
  
  
  SECTION("Dot Product")
  {
    const math::vec4 dot_vec_a = math::vec4_init(1.1f, 2.2f, 3.3f, 4.4f);
    const math::vec4 dot_vec_b = math::vec4_init(1.1f, 2.2f, 2.2f, 4.4f);
    const float dot_prod = math::vec4_dot(dot_vec_a, dot_vec_b);
    REQUIRE(math::is_near(dot_prod, 32.67f, error));
  }
  
  
  SECTION("Length")
  {
    const math::vec4 one_two = math::vec4_init(1.1f, 2.2f, 3.3f, 4.4f);
    const float length_of_vec = math::vec4_length(one_two);
    REQUIRE(math::is_near(length_of_vec, 6.02495f, error));
  }
  
}