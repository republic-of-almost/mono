#include <catch/catch.hpp>
#include <roa_math/math.h>
#include <array>


TEST_CASE("Vector 2")
{
	// Initialize test data that we will use
  roa_float2 one_two = roa_float2_set_with_values(1.f, 2.f);
  REQUIRE(roa_float2_get_x(one_two) == 1.f);
  REQUIRE(roa_float2_get_y(one_two) == 2.f);

  roa_float2 three = roa_float2_fill_with_value(3.f);
  REQUIRE(roa_float2_get_x(three) == 3.f);
  REQUIRE(roa_float2_get_y(three) == 3.f);

  const std::array<float, 2> arr_data = {4.f, 5.f};
  const roa_float2 four_five = roa_float2_import(&arr_data[0]);
  REQUIRE(roa_float2_get_x(four_five) == 4.f);
  REQUIRE(roa_float2_get_y(four_five) == 5.f);
  
  const float error = 0.0001f; // Error for near tests
  
  
  SECTION("Export data")
  {
    std::array<float, 2> arr_export = {0.f, 0.f};
    const roa_float2 to_export = roa_float2_one();
    roa_float2_export(to_export, arr_export.data());
    
    REQUIRE(arr_export.at(0) == 1.f);
    REQUIRE(arr_export.at(1) == 1.f);
  }


	SECTION("Constants")
	{
		const roa_float2 zero = roa_float2_zero();
    REQUIRE(roa_float2_get_x(zero) == 0.f);
    REQUIRE(roa_float2_get_y(zero) == 0.f);
    
    const roa_float2 one = roa_float2_one();
    REQUIRE(roa_float2_get_x(one) == 1.f);
    REQUIRE(roa_float2_get_y(one) == 1.f);
    
    const roa_float2 zero_one = roa_float2_zero_one();
    REQUIRE(roa_float2_get_x(zero_one) == 0.f);
    REQUIRE(roa_float2_get_y(zero_one) == 1.f);
	}
  
	/* 
  SECTION("Equality")
  {
    REQUIRE(math::vec2_is_equal(math::vec2_one(), math::vec2_one()));
    REQUIRE(math::vec2_is_not_equal(math::vec2_one(), math::vec2_zero()));
    
    REQUIRE(math::vec2_is_near(math::vec2_zero(), math::vec2_one(), 1.f));
    REQUIRE(math::vec2_is_not_near(math::vec2_zero(), math::vec2_one(), 0.5f));
  }
  
  
  SECTION("Basic Arithmetic")
  {
    const math::vec2 addition = math::vec2_add(one_two, three);
    REQUIRE(math::vec2_is_near(addition, four_five, error));
    
    const math::vec2 subtraction = math::vec2_subtract(addition, three);
    REQUIRE(math::vec2_is_near(subtraction, one_two, error));
    
    const math::vec2 mulitplication = math::vec2_multiply(one_two, four_five);
    const math::vec2 expected_multi = math::vec2_init(4.f, 10.f);
    REQUIRE(math::vec2_is_near(expected_multi, mulitplication, error));
    
    const math::vec2 division = math::vec2_divide(expected_multi, four_five);
    REQUIRE(math::vec2_is_near(division, one_two, error));
  }
  
  
  SECTION("Scale")
  {
    const math::vec2 scale_a = math::vec2_scale(one_two, 3.f);
    const math::vec2 expected_a = math::vec2_init(3.f, 6.f);
    REQUIRE(math::vec2_is_near(scale_a, expected_a, error));
    
    const math::vec2 scale_b = math::vec2_scale(one_two, 0.5f);
    const math::vec2 expected_b = math::vec2_init(0.5f, 1.f);
    REQUIRE(math::vec2_is_near(scale_b, expected_b, error));
  }
  
  
  SECTION("Lerp")
  {
    const math::vec2 lerp_start = math::vec2_zero();
    const math::vec2 lerp_end   = math::vec2_one();

    const math::vec2 dt_0 = math::vec2_lerp(lerp_start, lerp_end, 0.f);
    REQUIRE(math::vec2_is_near(dt_0, math::vec2_zero(), error));

    const math::vec2 dt_025 = math::vec2_lerp(lerp_start, lerp_end, 0.25f);
    REQUIRE(math::vec2_is_near(dt_025, math::vec2_init(0.25f, 0.25f), error));

    const math::vec2 dt_05 = math::vec2_lerp(lerp_start, lerp_end, 0.5f);
    REQUIRE(math::vec2_is_near(dt_05, math::vec2_init(0.5f, 0.5f), error));

    const math::vec2 dt_075 = math::vec2_lerp(lerp_start, lerp_end, 0.75f);
    REQUIRE(math::vec2_is_near(dt_075, math::vec2_init(0.75f, 0.75f), error));

    const math::vec2 dt_1 = math::vec2_lerp(lerp_start, lerp_end, 1.f);
    REQUIRE(math::vec2_is_near(dt_1, math::vec2_one(), error));
  }
  
  
  SECTION("Normalize")
  {
    const math::vec2 normalize_vec = math::vec2_normalize(one_two);
    REQUIRE(math::vec2_is_near(normalize_vec, math::vec2_init(0.4472f, 0.89442f), error));
  }
  */
  
  SECTION("Dot Product")
  {
    const float dot_prod = roa_float2_dot(roa_float2_set_with_values(1.1f, 2.2f), roa_float2_fill_with_value(2.2f));
    REQUIRE(roa_float_is_near(dot_prod, 7.26f, error));
  }
  
  
	/*
  SECTION("Length")
  {
    const math::vec2 one_two = math::vec2_init(1.1f, 2.2f);
    const float length_of_vec = math::vec2_length(one_two);
    REQUIRE(math::is_near(length_of_vec, 2.4596f, error));
  }
	*/
}
