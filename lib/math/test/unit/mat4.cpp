#include <catch/catch.hpp>
#include <math/mat/mat4.hpp>
#include <array>


TEST_CASE("Matrix 4x4")
{
  
  // ---------------------------------------------------------- [ Constants ] --

  SECTION("ID Mat")
  {
    const float mat_data[] {
      1.f, 0.f, 0.f, 0.f,
      0.f, 1.f, 0.f, 0.f,
      0.f, 0.f, 1.f, 0.f,
      0.f, 0.f, 0.f, 1.f,
    };
    
    const math::mat4 mat_expected = math::mat4_init(mat_data);
    const math::mat4 mat = math::mat4_id();
    
    REQUIRE(math::mat4_is_near(mat_expected, mat));
  }
  
  SECTION("Zero Mat")
  {
    const float mat_data[] {
      0.f, 0.f, 0.f, 0.f,
      0.f, 0.f, 0.f, 0.f,
      0.f, 0.f, 0.f, 0.f,
      0.f, 0.f, 0.f, 0.f,
    };
    
    const math::mat4 mat_expected = math::mat4_init(mat_data);
    const math::mat4 mat = math::mat4_zero();
    
    REQUIRE(math::mat4_is_near(mat_expected, mat));
  }
  
  // --------------------------------------------------------- [ Initialize ] --
  
  SECTION("Init With Nothing")
  {
    const math::mat4 mat_default_init = math::mat4_init();
    
    REQUIRE(math::mat4_is_near(mat_default_init, math::mat4_id()));
  }
  
  SECTION("Init with value")
  {
    const float mat_data[] {
      3.f, 3.f, 3.f, 3.f,
      3.f, 3.f, 3.f, 3.f,
      3.f, 3.f, 3.f, 3.f,
      3.f, 3.f, 3.f, 3.f,
    };
    
    const math::mat4 mat_expected = math::mat4_init(mat_data);
    const math::mat4 mat_init_with_value = math::mat4_init(3.f);
    
    REQUIRE(math::mat4_is_near(mat_expected, mat_init_with_value));
  }
  
  SECTION("Init with sub mat")
  {
    const float mat_expected_data[] {
      4.f, 4.f, 4.f, 0.f,
      4.f, 4.f, 4.f, 0.f,
      4.f, 4.f, 4.f, 0.f,
      0.f, 0.f, 0.f, 0.f,
    };
    
    const math::mat4 mat_expected = math::mat4_init(mat_expected_data);
    
    const float mat_sub_data[] {
      4.f, 4.f, 4.f,
      4.f, 4.f, 4.f,
      4.f, 4.f, 4.f,
    };
    
    const math::mat3 sub_mat = math::mat3_init(mat_sub_data);
    const math::mat4 mat_init_with_sub = math::mat4_init(sub_mat);
    
    REQUIRE(math::mat4_is_near(mat_expected, mat_init_with_sub));
  }
  
  SECTION("Init with array")
  {
    const float mat_data_1[] {
      1.f, 1.f, 1.f, 1.f,
      1.f, 1.f, 1.f, 1.f,
      1.f, 1.f, 1.f, 1.f,
      1.f, 1.f, 1.f, 1.f,
    };
    
    const math::mat4 mat_a = math::mat4_init(mat_data_1);
    
    const float mat_data_2[] {
      2.f, 2.f, 2.f, 2.f,
      2.f, 2.f, 2.f, 2.f,
      2.f, 2.f, 2.f, 2.f,
      2.f, 2.f, 2.f, 2.f,
    };
    
    const math::mat4 mat_b = math::mat4_init(mat_data_2);
    
    REQUIRE(math::mat4_is_near(mat_a, mat_b) != true);
  }
  
  // -------------------------------------------------- [ View / Projection ] --
  
  SECTION("Look at matrix")
  {
    /*
      Need some valid test data for this.
    */
  }
  
  SECTION("Perspective")
  {
    /*
      Need some valid test data for this.
    */
  }
  
  SECTION("Orthographic")
  {
    /*
      Need some valid test data for this.
    */
  }
  
  // --------------------------------------------------------- [ Operations ] --
  
  SECTION("Addition")
  {
    const float mat_data_1[] {
      1.f, 1.f, 1.f, 1.f,
      1.f, 1.f, 1.f, 1.f,
      1.f, 1.f, 1.f, 1.f,
      1.f, 1.f, 1.f, 1.f,
    };
    
    const math::mat4 mat_a = math::mat4_init(mat_data_1);
    
    const float mat_data_2[] {
      2.f, 2.f, 2.f, 2.f,
      2.f, 2.f, 2.f, 2.f,
      2.f, 2.f, 2.f, 2.f,
      2.f, 2.f, 2.f, 2.f,
    };
    
    const math::mat4 mat_b = math::mat4_init(mat_data_2);
    
    const float mat_expected_data[] {
      3.f, 3.f, 3.f, 3.f,
      3.f, 3.f, 3.f, 3.f,
      3.f, 3.f, 3.f, 3.f,
      3.f, 3.f, 3.f, 3.f,
    };
    
    const math::mat4 mat_expected = math::mat4_init(mat_expected_data);
    const math::mat4 mat_final = math::mat4_add(mat_a, mat_b);
    
    REQUIRE(math::mat4_is_near(mat_expected, mat_final));
  }
  
  SECTION("Subtraction")
  {
    const float mat_data_1[] {
      1.f, 1.f, 1.f, 1.f,
      1.f, 1.f, 1.f, 1.f,
      1.f, 1.f, 1.f, 1.f,
      1.f, 1.f, 1.f, 1.f,
    };
    
    const math::mat4 mat_a = math::mat4_init(mat_data_1);
    
    const float mat_data_2[] {
      2.f, 2.f, 2.f, 2.f,
      2.f, 2.f, 2.f, 2.f,
      2.f, 2.f, 2.f, 2.f,
      2.f, 2.f, 2.f, 2.f,
    };
    
    const math::mat4 mat_b = math::mat4_init(mat_data_2);
    
    const float mat_expected_data[] {
      -1.f, -1.f, -1.f, -1.f,
      -1.f, -1.f, -1.f, -1.f,
      -1.f, -1.f, -1.f, -1.f,
      -1.f, -1.f, -1.f, -1.f,
    };
    
    const math::mat4 mat_expected = math::mat4_init(mat_expected_data);
    const math::mat4 mat_final = math::mat4_subtract(mat_a, mat_b);
    
    REQUIRE(math::mat4_is_near(mat_expected, mat_final));
  }

  SECTION("Multiplication (float * mat)")
  {
    const float mat_data_1[] {
      1.f, 2.f, 3.f, 4.f,
      1.f, 2.f, 3.f, 4.f,
      1.f, 2.f, 3.f, 4.f,
      1.f, 2.f, 3.f, 4.f,
    };
    
    const math::mat4 mat_a = math::mat4_init(mat_data_1);
    
    const float mat_expected_data[] {
      10.f, 20.f, 30.f, 40.f,
      10.f, 20.f, 30.f, 40.f,
      10.f, 20.f, 30.f, 40.f,
      10.f, 20.f, 30.f, 40.f,
    };
    
    const math::mat4 mat_expected = math::mat4_init(mat_expected_data);
    const math::mat4 mat_final = math::mat4_multiply(10.f, mat_a);
    
    REQUIRE(math::mat4_is_near(mat_expected, mat_final));
  }
  
  SECTION("Multiplication (vec * mat)")
  {
    const math::vec4 val = math::vec4_init(2.f);
    
    const float mat_data[] {
      1.f, 2.f, 3.f, 4.f,
      1.f, 2.f, 3.f, 4.f,
      1.f, 2.f, 3.f, 4.f,
      1.f, 2.f, 3.f, 4.f,
    };
    
    const math::mat4 mat_a = math::mat4_init(mat_data);
    const math::vec4 result = math::mat4_multiply(val, mat_a);
    const math::vec4 expected = math::vec4_init(8.f, 16.f, 24.f, 32.f);
    
    REQUIRE(math::vec4_is_near(result, expected));
  }
  
  SECTION("Multiplication (mat * mat)")
  {
    const float mat_data_1[] {
      1.f, 2.f, 3.f, 4.f,
      1.f, 2.f, 3.f, 4.f,
      1.f, 2.f, 3.f, 4.f,
      1.f, 2.f, 3.f, 4.f,
    };
    
    const math::mat4 mat_a = math::mat4_init(mat_data_1);
    
    const float mat_data_2[] {
      1.f, 2.f, 3.f, 4.f,
      1.f, 2.f, 3.f, 4.f,
      1.f, 2.f, 3.f, 4.f,
      1.f, 2.f, 3.f, 4.f,
    };
    
    const math::mat4 mat_b = math::mat4_init(mat_data_2);
    
    const float mat_expected_data[] {
      10.f, 20.f, 30.f, 40.f,
      10.f, 20.f, 30.f, 40.f,
      10.f, 20.f, 30.f, 40.f,
      10.f, 20.f, 30.f, 40.f,
    };
    
    const math::mat4 mat_expected = math::mat4_init(mat_expected_data);
    const math::mat4 mat_final = math::mat4_multiply(mat_a, mat_b);
    
    REQUIRE(math::mat4_is_near(mat_expected, mat_final));
  }
  
  SECTION("Multiplication (mat * mat * mat)")
  {
    const float mat_data_1[] {
      1.f, 2.f, 3.f, 4.f,
      1.f, 2.f, 3.f, 4.f,
      1.f, 2.f, 3.f, 4.f,
      1.f, 2.f, 3.f, 4.f,
    };
    
    const math::mat4 mat_a = math::mat4_init(mat_data_1);
    
    const float mat_data_2[] {
      1.f, 2.f, 3.f, 4.f,
      1.f, 2.f, 3.f, 4.f,
      1.f, 2.f, 3.f, 4.f,
      1.f, 2.f, 3.f, 4.f,
    };
    
    const math::mat4 mat_b = math::mat4_init(mat_data_2);
    
    const float mat_data_3[] {
      1.f, 2.f, 3.f, 4.f,
      1.f, 2.f, 3.f, 4.f,
      1.f, 2.f, 3.f, 4.f,
      1.f, 2.f, 3.f, 4.f,
    };
    
    const math::mat4 mat_c = math::mat4_init(mat_data_3);
    
    const float mat_expected_data[] {
      100.f, 200.f, 300.f, 400.f,
      100.f, 200.f, 300.f, 400.f,
      100.f, 200.f, 300.f, 400.f,
      100.f, 200.f, 300.f, 400.f,
    };
    
    const math::mat4 mat_expected = math::mat4_init(mat_expected_data);
    const math::mat4 mat_final = math::mat4_multiply(mat_a, mat_b, mat_c);
    
    REQUIRE(math::mat4_is_near(mat_expected, mat_final));
  }
}
