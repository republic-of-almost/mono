#include <catch/catch.hpp>
#include <math/general/general.hpp>


namespace {

struct Math_general_test_data
{
  float input;
  float expected;
};


} // anons ns


TEST_CASE("General")
{
  // --------------------------------------------------------------- [ Trig ] --
  
  SECTION("Tan")
  {
    Math_general_test_data test_data[] {
      { 0.f, 0.f, },
      { -10.f, -0.64836082745f, },
      { 0.5f, 0.54630248984f, },
    };
    
    for(Math_general_test_data td : test_data)
    {
      const float actual = math::tan(td.input);
      REQUIRE(math::is_near(actual, td.expected, 0.00001f));
    }
  }
  
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
  
  SECTION("Sin")
  {
     Math_general_test_data test_data[] {
      { 0.f, 0.f, },
      { -10.f, 0.54402111088f, },
      { 0.5f, 0.4794255386f, },
    };
    
    for(Math_general_test_data td : test_data)
    {
      const float actual = math::sin(td.input);
      REQUIRE(math::is_near(actual, td.expected, 0.00001f));
    }
  }
  
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
  
  SECTION("Cos")
  {
     Math_general_test_data test_data[] {
      { 0.f, 1.f, },
      { -10.f, -0.83907152907f, },
      { 0.5f, 0.87758256189f, },
    };
    
    for(Math_general_test_data td : test_data)
    {
      const float actual = math::cos(td.input);
      REQUIRE(math::is_near(actual, td.expected, 0.00001f));
    }
  }
  
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
  
  // -------------------------------------------------- [ Degrees / Radians ] --
  
  SECTION("Rad/Deg Conversion")
  {
    const float rads = 0.123f;
    const float degs = 123.f;
    
    const float to_degs = math::to_degrees(rads);
    const float to_rads = math::to_radians(degs);
    
    REQUIRE(math::is_near(to_degs, 7.0473809f));
    REQUIRE(math::is_near(to_rads, 2.14675497995f));
  }
  
  // ------------------------------------------------------------ [ Casting ] --
  
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
  
  // ------------------------------------------------------------- [ Random ] --
  
  SECTION("Random")
  {
    /*
      Require data and a choice on how rand will work.
    */
  }
  
  // ---------------------------------------------------------- [ Min / Max ] --
  
  SECTION("Min/Max float")
  {
    REQUIRE(math::min(1.23f, 2.23f) < 2.23f);
    REQUIRE(math::max(1.23f, 2.23f) > 1.23f);
  }
  
  SECTION("Min/Max int")
  {
  }
  
  SECTION("Min/Max uint")
  {
  }
  
  SECTION("Min/Max uint64")
  {
  }
  
  SECTION("Min/Max Length")
  {
  }
  
  SECTION("Min/Max In Array")
  {
  }

  // ----------------------------------------------------------- [ Rounding ] --
  
  SECTION("Min/Max Length")
  {
  }
  
  SECTION("Min/Max Length")
  {
  }
  
  // --------------------------------------------------------- [ Arithmetic ] --
  
  SECTION("Sum")
  {
  }
  
  // --------------------------------------------------------------- [ Misc ] --

  SECTION("Abs")
  {
  }
  
  SECTION("Sqrt")
  {
  }
  
  SECTION("Clamp")
  {
  }
  
  SECTION("Sign")
  {
  }
  
  SECTION("Mod")
  {
  }
  
  // ----------------------------------------------------------- [ Equality ] --
  
  SECTION("Is between")
  {
  }
  
  SECTION("Is Near")
  {
  }
  
  SECTION("Is POW 2")
  {
  }
  
}
