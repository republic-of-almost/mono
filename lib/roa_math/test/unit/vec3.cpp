#include <catch/catch.hpp>
#include <roa_math/math.h>
#include <array>


TEST_CASE("Vector 3")
{
  
	// Initialize test data that we will use
  roa_float3 one_two_three = roa_float3_set_with_values(1.f, 2.f, 3.f);
  REQUIRE(roa_float3_get_x(one_two_three) == 1.f);
  REQUIRE(roa_float3_get_y(one_two_three) == 2.f);
  REQUIRE(roa_float3_get_z(one_two_three) == 3.f);

  roa_float3 four = roa_float3_fill_with_value(4.f);
  REQUIRE(roa_float3_get_x(four) == 4.f);
  REQUIRE(roa_float3_get_y(four) == 4.f);
  REQUIRE(roa_float3_get_z(four) == 4.f);

  const float arr_data[] {5.f, 6.f, 7.f};
  const roa_float3 five_six_seven = roa_float3_import(&arr_data[0]);
  REQUIRE(roa_float3_get_x(five_six_seven) == 5.f);
  REQUIRE(roa_float3_get_y(five_six_seven) == 6.f);
  REQUIRE(roa_float3_get_z(five_six_seven) == 7.f);
  
  const float error = 0.0001f; // Error for near tests

	
  SECTION("Export Data")
  {
    float arr_export[3]{};
    const roa_float3 to_export = roa_float3_one();
    roa_float3_export(to_export, arr_export);
    
    REQUIRE(arr_export[0] == 1.f);
    REQUIRE(arr_export[1] == 1.f);
    REQUIRE(arr_export[2] == 1.f);
  }
  
	
	SECTION("Constants")
	{
		const roa_float3 zero = roa_float3_zero();
    REQUIRE(roa_float3_get_x(zero) == 0.f);
    REQUIRE(roa_float3_get_y(zero) == 0.f);
    REQUIRE(roa_float3_get_z(zero) == 0.f);
    
    const roa_float3 one = roa_float3_one();
    REQUIRE(roa_float3_get_x(one) == 1.f);
    REQUIRE(roa_float3_get_y(one) == 1.f);
    REQUIRE(roa_float3_get_z(one) == 1.f);
    
    const roa_float3 zero_zero_one = roa_float3_zero_zero_one();
    REQUIRE(roa_float3_get_x(zero_zero_one) == 0.f);
    REQUIRE(roa_float3_get_y(zero_zero_one) == 0.f);
    REQUIRE(roa_float3_get_z(zero_zero_one) == 1.f);
	}
  
	 
  SECTION("Equality")
  {
    REQUIRE(roa_float3_is_near(roa_float3_one(), roa_float3_one(), error));
    REQUIRE(roa_float3_is_near(roa_float3_one(), roa_float3_zero(), error) == 0);
    
    REQUIRE(roa_float3_is_near(roa_float3_zero(), roa_float3_one(), 1.f));
    REQUIRE(roa_float3_is_near(roa_float3_zero(), roa_float3_one(), 0.5f) == 0);
  }
  
	 
  SECTION("Basic Arithmetic")
  {
    roa_float3 addition = roa_float3_add(one_two_three, four);
    REQUIRE(roa_float3_is_near(addition, five_six_seven, error));
    
    roa_float3 subtraction = roa_float3_subtract(addition, four);
    REQUIRE(roa_float3_is_near(subtraction, one_two_three, error));
    
    roa_float3 mulitplication = roa_float3_multiply(one_two_three, five_six_seven);
    roa_float3 expected_multi = roa_float3_set_with_values(5.f, 12.f, 21.f);
    REQUIRE(roa_float3_is_near(expected_multi, mulitplication, error));
   
		/*
    roa_float3 division = roa_float3_divide(expected_multi, five_six_seven);
    REQUIRE(roa_float3_is_near(division, one_two_three, error));
		*/
  }
  
	 
  SECTION("Scale")
  {
    const roa_float3 scale_a = roa_float3_scale(one_two_three, 3.f);
    const roa_float3 expected_a = roa_float3_set_with_values(3.f, 6.f, 9.f);
    REQUIRE(roa_float3_is_near(scale_a, expected_a, error));
    
    const roa_float3 scale_b = roa_float3_scale(one_two_three, 0.5f);
    const roa_float3 expected_b = roa_float3_set_with_values(0.5f, 1.f, 1.5f);
    REQUIRE(roa_float3_is_near(scale_b, expected_b, error));
  }
  
	 
  SECTION("Lerp")
  {
    const roa_float3 lerp_start = roa_float3_zero();
    const roa_float3 lerp_end   = roa_float3_one();

    const roa_float3 dt_0 = roa_float3_lerp(lerp_start, lerp_end, 0.f);
    REQUIRE(roa_float3_is_near(dt_0, roa_float3_zero(), error));

    const roa_float3 dt_025 = roa_float3_lerp(lerp_start, lerp_end, 0.25f);
    REQUIRE(roa_float3_is_near(dt_025, roa_float3_set_with_values(0.25f, 0.25f, 0.25f), error));

    const roa_float3 dt_05 = roa_float3_lerp(lerp_start, lerp_end, 0.5f);
    REQUIRE(roa_float3_is_near(dt_05, roa_float3_set_with_values(0.5f, 0.5f, 0.5f), error));

    const roa_float3 dt_075 = roa_float3_lerp(lerp_start, lerp_end, 0.75f);
    REQUIRE(roa_float3_is_near(dt_075, roa_float3_set_with_values(0.75f, 0.75f, 0.75f), error));

    const roa_float3 dt_1 = roa_float3_lerp(lerp_start, lerp_end, 1.f);
    REQUIRE(roa_float3_is_near(dt_1, roa_float3_one(), error));
  }
  
	
  SECTION("Normalize")
  {
    const roa_float3 unnormilized_vec = roa_float3_set_with_values(1.1f, 2.2f, 3.3f);
    const roa_float3 normalize_vec = roa_float3_normalize(unnormilized_vec);
    REQUIRE(roa_float3_is_near(normalize_vec, roa_float3_set_with_values(0.2672f, 0.5345, 0.80178), error));
  }
  
  
  SECTION("Dot Product")
  {
    const roa_float3 dot_vec_a = roa_float3_set_with_values(1.1f, 2.2f, 3.3f);
    const roa_float3 dot_vec_b = roa_float3_set_with_values(1.1f, 2.2f, 2.2f);
    const float dot_prod = roa_float3_dot(dot_vec_a, dot_vec_b);
    REQUIRE(roa_float_is_near(dot_prod, 13.31f, error));
  }
  
	 
  SECTION("Length")
  {
    const roa_float3 one_two = roa_float3_set_with_values(1.1f, 2.2f, 3.3f);
    const float length_of_vec = roa_float3_length(one_two);
    REQUIRE(roa_float_is_near(length_of_vec, 4.11582f, error));
  }
  
  
  SECTION("Cross")
  {
    const roa_float3 cross_vec_a = roa_float3_set_with_values(1.1f, 2.2f, 3.3f);
    const roa_float3 cross_vec_b = roa_float3_set_with_values(1.1f, 2.2f, 2.2f);
    const roa_float3 cross = roa_float3_cross(cross_vec_a, cross_vec_b);
    REQUIRE(roa_float3_is_near(cross, roa_float3_set_with_values(-2.42f, 1.21f, 0.f), error));
  }
}
