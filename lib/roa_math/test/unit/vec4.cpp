#include <catch/catch.hpp>
#include <roa_math/math.h>
#include <array>
#include <stdio.h>


TEST_CASE("Vector 4")
{
  // Initialize test data that we will use
  const roa_float4 one_two_three_four = roa_float4_set_with_values(1.f, 2.f, 3.f, 4.f);
  REQUIRE(roa_float4_get_x(one_two_three_four) == 1.f);
  REQUIRE(roa_float4_get_y(one_two_three_four) == 2.f);
  REQUIRE(roa_float4_get_z(one_two_three_four) == 3.f);
  REQUIRE(roa_float4_get_w(one_two_three_four) == 4.f);

  const roa_float4 four = roa_float4_fill_with_value(4.f);
  REQUIRE(roa_float4_get_x(four) == 4.f);
  REQUIRE(roa_float4_get_y(four) == 4.f);
  REQUIRE(roa_float4_get_z(four) == 4.f);
  REQUIRE(roa_float4_get_w(four) == 4.f);

  const float arr_data[] {5.f, 6.f, 7.f, 8.f};
  const roa_float4 five_six_seven_eight = roa_float4_import(&arr_data[0]);
  REQUIRE(roa_float4_get_x(five_six_seven_eight) == 5.f);
  REQUIRE(roa_float4_get_y(five_six_seven_eight) == 6.f);
  REQUIRE(roa_float4_get_z(five_six_seven_eight) == 7.f);
  REQUIRE(roa_float4_get_w(five_six_seven_eight) == 8.f);
  
  const float error = 0.0001f; // Error for near tests


  SECTION("Export Data")
  {
    float arr_export[4]{};
    const roa_float4 to_export = roa_float4_one();
    roa_float4_export(to_export, arr_export);
    
    REQUIRE(arr_export[0] == 1.f);
    REQUIRE(arr_export[1] == 1.f);
    REQUIRE(arr_export[2] == 1.f);
    REQUIRE(arr_export[3] == 1.f);
  }
  

	SECTION("Constants")
	{
		const roa_float4 zero = roa_float4_zero();
    REQUIRE(roa_float4_get_x(zero) == 0.f);
    REQUIRE(roa_float4_get_y(zero) == 0.f);
    REQUIRE(roa_float4_get_z(zero) == 0.f);
    REQUIRE(roa_float4_get_w(zero) == 0.f);
    
    const roa_float4 one = roa_float4_one();
    REQUIRE(roa_float4_get_x(one) == 1.f);
    REQUIRE(roa_float4_get_y(one) == 1.f);
    REQUIRE(roa_float4_get_z(one) == 1.f);
    REQUIRE(roa_float4_get_w(one) == 1.f);
    
    const roa_float4 zero_zero_zero_one = roa_float4_zero_zero_zero_one();
    REQUIRE(roa_float4_get_x(zero_zero_zero_one) == 0.f);
    REQUIRE(roa_float4_get_y(zero_zero_zero_one) == 0.f);
    REQUIRE(roa_float4_get_z(zero_zero_zero_one) == 0.f);
    REQUIRE(roa_float4_get_w(zero_zero_zero_one) == 1.f);

	}
  
  
  SECTION("Equality")
  {
  /*  REQUIRE(roa_float4_is_equal(roa_float4_one(), roa_float4_one()));
    REQUIRE(roa_float4_is_not_equal(roa_float4_one(), roa_float4_zero()));
    
    REQUIRE(roa_float4_is_near(roa_float4_zero(), roa_float4_one(), 1.f));
    REQUIRE(roa_float4_is_not_near(roa_float4_zero(), roa_float4_one(), 0.5f));
		*/
  }
  
  
  SECTION("Basic Arithmetic")
  {
    const roa_float4 addition = roa_float4_add(one_two_three_four, four);
    REQUIRE(roa_float4_is_near(addition, five_six_seven_eight, error));
    
    const roa_float4 subtraction = roa_float4_subtract(addition, four);
    REQUIRE(roa_float4_is_near(subtraction, one_two_three_four, error));
    
    const roa_float4 mulitplication = roa_float4_multiply(one_two_three_four, five_six_seven_eight);
    const roa_float4 expected_multi = roa_float4_set_with_values(5.f, 12.f, 21.f, 32.f);
    REQUIRE(roa_float4_is_near(expected_multi, mulitplication, error));
   /* 
    const roa_float4 division = roa_float4_divide(expected_multi, five_six_seven_eight);
    REQUIRE(roa_float4_is_near(division, one_two_three_four, error));
		*/
  }
  
  
  SECTION("Scale")
  {
    const roa_float4 scale_a = roa_float4_scale(one_two_three_four, 3.f);
    const roa_float4 expected_a = roa_float4_set_with_values(3.f, 6.f, 9.f, 12.f);
    REQUIRE(roa_float4_is_near(scale_a, expected_a, error));
    
    const roa_float4 scale_b = roa_float4_scale(one_two_three_four, 0.5f);
    const roa_float4 expected_b = roa_float4_set_with_values(0.5f, 1.f, 1.5f, 2.f);
    REQUIRE(roa_float4_is_near(scale_b, expected_b, error));
  }
  
  
  SECTION("Lerp")
  {
    const roa_float4 lerp_start = roa_float4_zero();
    const roa_float4 lerp_end   = roa_float4_one();

    const roa_float4 dt_0 = roa_float4_lerp(lerp_start, lerp_end, 0.f);
    REQUIRE(roa_float4_is_near(dt_0, roa_float4_zero(), error));

    const roa_float4 dt_025 = roa_float4_lerp(lerp_start, lerp_end, 0.25f);
    REQUIRE(roa_float4_is_near(dt_025, roa_float4_set_with_values(0.25f, 0.25f, 0.25f, 0.25), error));

    const roa_float4 dt_05 = roa_float4_lerp(lerp_start, lerp_end, 0.5f);
    REQUIRE(roa_float4_is_near(dt_05, roa_float4_set_with_values(0.5f, 0.5f, 0.5f, 0.5f), error));

    const roa_float4 dt_075 = roa_float4_lerp(lerp_start, lerp_end, 0.75f);
    REQUIRE(roa_float4_is_near(dt_075, roa_float4_set_with_values(0.75f, 0.75f, 0.75f, 0.75f), error));

    const roa_float4 dt_1 = roa_float4_lerp(lerp_start, lerp_end, 1.f);
    REQUIRE(roa_float4_is_near(dt_1, roa_float4_one(), error));
  }
  

  SECTION("Normalize")
  {
    const roa_float4 unnormilized_vec = roa_float4_set_with_values(1.1f, 2.2f, 3.3f, 4.4f);
    const roa_float4 normalize_vec = roa_float4_normalize(unnormilized_vec);
    REQUIRE(roa_float4_is_near(normalize_vec, roa_float4_set_with_values(0.182574f, 0.365148f, 0.547723f, 0.730297f), error));
  }
  
  
  SECTION("Dot Product")
  {
    const roa_float4 dot_vec_a = roa_float4_set_with_values(1.1f, 2.2f, 3.3f, 4.4f);
    const roa_float4 dot_vec_b = roa_float4_set_with_values(1.1f, 2.2f, 2.2f, 4.4f);
    const float dot_prod = roa_float4_dot(dot_vec_a, dot_vec_b);
    REQUIRE(roa_float_is_near(dot_prod, 32.67f, error));
  }
  
  
  SECTION("Length")
  {
    const roa_float4 one_two = roa_float4_set_with_values(1.1f, 2.2f, 3.3f, 4.4f);
    const float length_of_vec = roa_float4_length(one_two);

		REQUIRE(roa_float_is_near(length_of_vec, 6.02495f, error));
  }
}
