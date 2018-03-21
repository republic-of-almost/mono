#include <catch/catch.hpp>
#include <roa_math/math.h>


TEST_CASE("General")
{
  // --------------------------------------------------------------- [ Trig ] --

	
  SECTION("Tan")
  {
    float res_1 = roa_float_tan(0.f);
    REQUIRE(roa_float_is_near(res_1, 0.f, ROA_EPSILON));

    float res_2 = roa_float_tan(-10.f);
    REQUIRE(roa_float_is_near(res_2, -0.64836082745f, ROA_EPSILON));

    float res_3 = roa_float_tan(0.5f);
    REQUIRE(roa_float_is_near(res_3, 0.54630248984f, ROA_EPSILON));
  }
	
  
	/*
  SECTION("A Tan2")
  {
    Math_general_test_data test_data[] {
      { 0.f, 0.f, },
      { 1.f, 0.463647609f, },
      { 2.f, 0.785398163f, },
    };
    
    for(Math_general_test_data td : test_data)
    {
      const float actual = math::a_tan2(td.input, 2.f);
      REQUIRE(math::is_near(actual, td.expected, 0.00001f));
    }
  }
	*/
  
	
  SECTION("Sin")
  {    
    float res_1 = roa_float_sin(0.f);
    REQUIRE(roa_float_is_near(res_1, 0.f, ROA_EPSILON));

    float res_2 = roa_float_sin(-10.f);
    REQUIRE(roa_float_is_near(res_2, 0.54402111088f, ROA_EPSILON));

    float res_3 = roa_float_sin(0.5f);
    REQUIRE(roa_float_is_near(res_3, 0.4794255386f, ROA_EPSILON));
  }
	
  
	/*
  SECTION("A Sin")
  {
    Math_general_test_data test_data[] {
      { 0.f, 0.f, },
      { 0.4f, 0.411516846f, },
      { -0.4f, -0.411516846f, },
    };
    
    for(Math_general_test_data td : test_data)
    {
      const float actual = math::a_sin(td.input);
      REQUIRE(math::is_near(actual, td.expected, 0.00001f));
    }
  }
	*/


  SECTION("Cos")
  {
    float res_1 = roa_float_cos(0.f);
    REQUIRE(roa_float_is_near(res_1, 1.f, ROA_EPSILON));

    float res_2 = roa_float_cos(-10.f);
    REQUIRE(roa_float_is_near(res_2, -0.83907152907f, ROA_EPSILON));

    float res_3 = roa_float_cos(0.5f);
    REQUIRE(roa_float_is_near(res_3, 0.87758256189f, ROA_EPSILON));
  }
	
  
	/*
  SECTION("A Cos")
  {
    Math_general_test_data test_data[] {
      { 0.f, 1.57079633f, },
      { 0.4f, 1.15927948f, },
      { -0.4f, 1.98231317f, },
    };
    
    for(Math_general_test_data td : test_data)
    {
      const float actual = math::a_cos(td.input);
      REQUIRE(math::is_near(actual, td.expected, 0.00001f));
    }
  }
	*/
  
  // -------------------------------------------------- [ Degrees / Radians ] --
  
	/*
  SECTION("Rad/Deg Conversion")
  {
    const float rads = 0.123f;
    const float degs = 123.f;
    
    const float to_degs = math::to_degrees(rads);
    const float to_rads = math::to_radians(degs);
    
    REQUIRE(math::is_near(to_degs, 7.0473809f, 0.00001f));
    REQUIRE(math::is_near(to_rads, 2.14675497995f, 0.00001f));
  }
  */

  // ------------------------------------------------------------ [ Casting ] --
  
	/*
	SECTION("Casting")
  {
    const uint32_t to_flt_a = 123;
    const float to_flt_a_res = math::to_float(to_flt_a);
    REQUIRE(math::is_near(to_flt_a_res, 123.f));
    
    const int32_t to_flt_b = -123;
    const float to_flt_b_res = math::to_float(to_flt_b);
    REQUIRE(math::is_near(to_flt_b_res, -123.f));
    
    const float to_int_a = 123.3;
    const uint32_t to_int_a_res = math::to_uint(to_int_a);
    REQUIRE(math::is_near(math::to_uint(to_int_a), 123));
    
    const float to_int_b = -123.3;
    const int32_t to_int_b_res = math::to_int(to_int_b);
    REQUIRE(math::is_near(to_int_b_res, -123));
  }
	*/
  
  // ------------------------------------------------------------- [ Random ] --
  
	/*
  SECTION("Random")
  {
  }
	*/
  
  // ---------------------------------------------------------- [ Min / Max ] --
  
  /*
	SECTION("Min/Max float")
  {
    REQUIRE(math::min(1.23f, 2.23f) < 2.23f);
    REQUIRE(math::max(1.23f, 2.23f) > 1.23f);
  }
	*/
  
	/*
  SECTION("Min/Max int")
  {
    const int32_t a = -32;
    const int32_t b = 55;
    
    REQUIRE(math::min(a, b) < b);
    REQUIRE(math::max(a, b) > a);
  }
	*/

	/*  
  SECTION("Min/Max uint")
  {
    const uint32_t a = 32;
    const uint32_t b = 55;
    
    REQUIRE(math::min(a, b) < b);
    REQUIRE(math::max(a, b) > a);

  }
	*/
  
	/*
  SECTION("Min/Max uint64")
  {
    const uint64_t a = 32;
    const uint64_t b = 55;
    
    REQUIRE(math::min(a, b) < b);
  }
	*/
  
	/*
  SECTION("Min/Max In Array")
  {
    constexpr size_t count = 4;
  
    const float a[count] {1.f, 1.f, 1.f, 1.f};
    const float b[count] {2.f, 2.f, 2.f, 2.f};
    
    float out[count]{};
    
    math::max(out, a, b, count);
    
    for(uint32_t i = 0; i < count; ++i)
    {
      REQUIRE(out[i] == b[i]);
    }
    
    math::min(out, a, b, count);
    
    for(uint32_t i = 0; i < count; ++i)
    {
      REQUIRE(out[i] == a[i]);
    }
  }
	*/

  // ----------------------------------------------------------- [ Rounding ] --
  
	/*
  SECTION("Ceil")
  {
    const float a = 123.3;
    const float ceil_a = math::ceil(a);
    
    REQUIRE(ceil_a == 124.f);
    
    const float b = 123.7;
    const float ceil_b = math::ceil(b);
    
    REQUIRE(ceil_b == 124.f);
  }
	*/

	/*
  SECTION("Floor")
  {
    const float a = 123.3;
    const float floor_a = math::floor(a);
    
    REQUIRE(floor_a == 123.f);
    
    const float b = 123.7;
    const float floor_b = math::floor(b);
    
    REQUIRE(floor_b == 123.f);
  }
	*/
  
	/*
  SECTION("Floor nearest")
  {
    const float a = 123.5f;
    const float floor = math::nearest_floor(a, 0.2f);
    
    REQUIRE(floor == 123.4f);
  }
	*/
  
  // --------------------------------------------------------- [ Arithmetic ] --
  
	/*
  SECTION("Sum")
  {
    constexpr size_t count = 4;
  
    const float a[count] {1.f, 1.f, 1.f, 1.f};
    const float b[count] {1.f, 1.f, 1.f, 1.f};
    const float expected[count] {2.f, 2.f, 2.f, 2.f};
    
    float out[count]{};
    
    math::add(out, a, b, count);
    
    for(uint32_t i = 0; i < count; ++i)
    {
      REQUIRE(out[i] == expected[i]);
    }
  }
	*/
  
  // --------------------------------------------------------------- [ Misc ] --
  
	/*
  SECTION("Min/Max Length")
  {
    const float a = -100;
    const float b = 10;
    
    REQUIRE(math::max_length(a, b) == a);
    REQUIRE(math::min_length(a, b) == b);
  }
	*/

	
  SECTION("Abs")
  {
    REQUIRE(roa_float_abs(-123.f) == 123.f);
    REQUIRE(roa_float_abs(123.f) == 123.f);
  }
	
  
	
  SECTION("Sqrt")
  {
    float res = roa_float_sqrt(123.f);
    REQUIRE(roa_float_is_near(res, 11.09053650f, ROA_EPSILON));
  }
	
  
	/*
  SECTION("Clamp")
  {
    REQUIRE(math::clamp(567.5f, 123.f, 345.f) == 345.f);
    REQUIRE(math::clamp(1.f, 123.f, 345.f) == 123.f);
  }
	*/
  
	/*
  SECTION("Sign")
  {
    const float a = -1234.f;
    const float b = 1235.f;
    
    REQUIRE(math::sign(a) == -1.f);
    REQUIRE(math::sign(b) == +1.f);
  }
	*/
  
	/*
  SECTION("Mod")
  {
    const float a = 123.f;
    const float mod = math::mod(a, 100.f);
    
    REQUIRE(mod == 23.f);
  }
  */

  // ----------------------------------------------------------- [ Equality ] --

	/*
  SECTION("Is between")
  {
    REQUIRE(math::is_between(0, -1.f, 1.f) == true);
    REQUIRE(math::is_between(3, -1.f, 5.f) == true);
    REQUIRE(math::is_between(3.f, -1.f, 1.f) == false);
    REQUIRE(math::is_between(-8.f, -5.f, 1.f) == false);
  }
	*/
  
	
  SECTION("Is Near")
  {
    REQUIRE(!!roa_float_is_near(1.123f, 1.123f, ROA_EPSILON) == true);
    REQUIRE(!!roa_float_is_near(23.34f, 1.123f, ROA_EPSILON) == false);
  }
	
  
	/*
  SECTION("Is POW 2")
  {
    REQUIRE(math::is_pow_two(256) == true);
    REQUIRE(math::is_pow_two(512) == true);
    REQUIRE(math::is_pow_two(123) == false);
    REQUIRE(math::is_pow_two(456) == false);
  }
  */
}


