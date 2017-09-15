#include <catch/catch.hpp>
#include <math/mat/mat3.hpp>


TEST_CASE("Matrix 3x3")
{
  // ---------------------------------------------------------- [ Constants ] --
  
  SECTION("ID Mat")
  {
    const float mat_data[] {
      1.f, 0.f, 0.f,
      0.f, 1.f, 0.f,
      0.f, 0.f, 1.f,
    };
    
    const math::mat3 mat_expected = math::mat3_init(mat_data);
    const math::mat3 mat = math::mat3_id();
    
    REQUIRE(math::mat3_is_near(mat_expected, mat));
  }
  
  SECTION("Zero Mat")
  {
    const float mat_data[] {
      0.f, 0.f, 0.f,
      0.f, 0.f, 0.f,
      0.f, 0.f, 0.f,
    };
    
    const math::mat3 mat_expected = math::mat3_init(mat_data);
    const math::mat3 mat = math::mat3_zero();
    
    REQUIRE(math::mat3_is_near(mat_expected, mat));
  }
  
  // --------------------------------------------------------- [ Initialize ] --
  
  SECTION("Init with Nothing")
  {
    const math::mat3 mat_default_init = math::mat3_init();
    const math::mat3 id = math::mat3_init();
    
    REQUIRE(math::mat3_is_near(mat_default_init, id));
  }
  
  SECTION("Init with Value")
  {
    const float mat_data[] {
      3.f, 3.f, 3.f,
      3.f, 3.f, 3.f,
      3.f, 3.f, 3.f,
    };
    
    const math::mat3 mat_expected = math::mat3_init(mat_data);
    const math::mat3 mat = math::mat3_init(3.f);
    
    REQUIRE(math::mat3_is_near(mat_expected, mat));
  }
  
  SECTION("Init with Array")
  {
    const float mat_data_1[] {
      1.f, 1.f, 1.f,
      1.f, 1.f, 1.f,
      1.f, 1.f, 1.f,
    };
    
    const math::mat3 mat_a = math::mat3_init(mat_data_1);
    
    const float mat_data_2[] {
      2.f, 2.f, 2.f,
      2.f, 2.f, 2.f,
      2.f, 2.f, 2.f,
    };
    
    const math::mat3 mat_b = math::mat3_init(mat_data_2);
    
    REQUIRE(math::mat3_is_not_near(mat_a, mat_b));
  }
  
  // --------------------------------------------- [ Special Init With Data ] --
  
  SECTION("Scale Mat")
  {
    const float expected_mat_data[] {
      1.f, 0.f, 0.f,
      0.f, 2.f, 0.f,
      0.f, 0.f, 1.f,
    };
    
    const math::mat3 mat_expected = math::mat3_init(expected_mat_data);
    const math::mat3 mat_scale = math::mat3_scale(1.f, 2.f);
    
    REQUIRE(math::mat3_is_near(mat_expected, mat_scale));
  }
  
  SECTION("mat3 pitch from eular")
  {
    /*
      Need test data
    */
  }
  
  SECTION("mat3 yaw from eular")
  {
    /*
      Need test data
    */
  }
  
  SECTION("mat3 yaw from eular")
  {
    /*
      Need test data
    */
  }
  
  // --------------------------------------------------------- [ Operations ] --
  
  SECTION("Addition")
  {
    const float mat_a_data[] {
      1.f, 1.f, 1.f,
      1.f, 1.f, 1.f,
      1.f, 1.f, 1.f,
    };
    
    const math::mat3 mat_a = math::mat3_init(mat_a_data);
    
    const float mat_b_data[] {
      2.f, 2.f, 2.f,
      2.f, 2.f, 2.f,
      2.f, 2.f, 2.f,
    };
    
    const math::mat3 mat_b = math::mat3_init(mat_b_data);
    
    const float mat_c_data[] {
      3.f, 3.f, 3.f,
      3.f, 3.f, 3.f,
      3.f, 3.f, 3.f,
    };
    
    const math::mat3 mat_expected = math::mat3_init(mat_c_data);
    const math::mat3 mat_result = math::mat3_add(mat_a, mat_b);
    
    REQUIRE(math::mat3_is_near(mat_expected, mat_result));
  }
  
  SECTION("Subtraction")
  {
    const float mat_a_data[] {
      1.f, 1.f, 1.f,
      1.f, 1.f, 1.f,
      1.f, 1.f, 1.f,
    };
    
    const math::mat3 mat_a = math::mat3_init(mat_a_data);
    
    const float mat_b_data[] {
      2.f, 2.f, 2.f,
      2.f, 2.f, 2.f,
      2.f, 2.f, 2.f,
    };
    
    const math::mat3 mat_b = math::mat3_init(mat_b_data);
    
    const float mat_c_data[] {
      -1.f, -1.f, -1.f,
      -1.f, -1.f, -1.f,
      -1.f, -1.f, -1.f,
    };
    
    const math::mat3 mat_expected = math::mat3_init(mat_c_data);
    const math::mat3 mat_result = math::mat3_subtract(mat_a, mat_b);
    
    REQUIRE(math::mat3_is_near(mat_expected, mat_result));
  }
  
  SECTION("Multiplication (mat * mat)")
  {
    const float mat_a_data[] {
      1.f, 1.f, 1.f,
      1.f, 1.f, 1.f,
      1.f, 1.f, 1.f,
    };
    
    const math::mat3 mat_a = math::mat3_init(mat_a_data);
    
    const float mat_b_data[] {
      2.f, 2.f, 2.f,
      2.f, 2.f, 2.f,
      2.f, 2.f, 2.f,
    };
    
    const math::mat3 mat_b = math::mat3_init(mat_b_data);
    
    const float mat_c_data[] {
      6.f, 6.f, 6.f,
      6.f, 6.f, 6.f,
      6.f, 6.f, 6.f,
    };
    
    const math::mat3 mat_expected = math::mat3_init(mat_c_data);
    const math::mat3 mat_result = math::mat3_multiply(mat_a, mat_b);
    
    REQUIRE(math::mat3_is_near(mat_expected, mat_result));
  }
  
  SECTION("Multiplication (mat * vec)")
  {
    const float mat_a_data[] {
      1.f, 1.f, 1.f,
      1.f, 1.f, 1.f,
      1.f, 1.f, 1.f,
    };
    
    const math::mat3 mat_a = math::mat3_init(mat_a_data);
    const math::vec3 vec = math::vec3_init(1.f,2.f,3.f);
    
    const math::vec3 vec_expected = math::vec3_init(6.f,6.f,6.f);
    const math::vec3 vec_result = math::mat3_multiply(vec, mat_a);
    
    REQUIRE(math::vec3_is_near(vec_expected, vec_result));
  }
  
  SECTION("Transpose")
  {
    const float mat_data[] {
      1.f, 2.f, 3.f,
      1.f, 2.f, 3.f,
      1.f, 2.f, 3.f,
    };
    
    const math::mat3 mat_a = math::mat3_init(mat_data);
    const math::mat3 transpose = math::mat3_transpose(mat_a);
    
    const float mat_expected_data[] {
      1.f, 1.f, 1.f,
      2.f, 2.f, 2.f,
      3.f, 3.f, 3.f,
    };
    
    const math::mat3 mat_expected = math::mat3_init(mat_expected_data);
    
    REQUIRE(math::mat3_is_near(transpose, mat_expected));
  }
  
  SECTION("Determinant")
  {
  }
  
  // --------------------------------------------------------------- [ Data ] --
  
  SECTION("To Array")
  {
    const float expected_mat_data[] {
      0.f, 1.f, 2.f,
      3.f, 4.f, 5.f,
      6.f, 7.f, 8.f,
    };
    
    const math::mat3 mat_a = math::mat3_init(expected_mat_data);
    
    float export_mat[9]{};
    math::mat3_to_array(mat_a, export_mat);
    
    for(uint32_t i = 0; i < 9; ++i)
    {
      REQUIRE(expected_mat_data[i] == export_mat[i]);
    }
  }
  
  SECTION("Data")
  {
    const float expected_mat_data[] {
      0.f, 1.f, 2.f,
      3.f, 4.f, 5.f,
      6.f, 7.f, 8.f,
    };
    
    const math::mat3 mat_a = math::mat3_init(expected_mat_data);
    
    const float *mat_data = math::mat3_data(mat_a);
    
    for(uint32_t i = 0; i < 9; ++i)
    {
      REQUIRE(expected_mat_data[i] == mat_data[i]);
    }
  }
  
  // ----------------------------------------------------------- [ Equality ] --
  
  SECTION("Equal")
  {
    const float mat_data_a[] {
      1.f, 2.f, 3.f,
      1.f, 2.f, 3.f,
      1.f, 2.f, 3.f,
    };
    
    const math::mat3 mat_a = math::mat3_init(mat_data_a);
    
    const float mat_data_b[] {
      1.f, 2.f, 3.f,
      1.f, 2.f, 3.f,
      1.f, 2.f, 3.f,
      1.f, 2.f, 3.f,
    };
    
    const math::mat3 mat_b = math::mat3_init(mat_data_b);
    
    REQUIRE(math::mat3_is_near(mat_a, mat_b) == true);
    REQUIRE(math::mat3_is_not_near(mat_a, mat_b) == false);
  }
  
  SECTION("Not Equal")
  {
    const float mat_data_a[] {
      5.f, 6.f, 7.f,
      5.f, 6.f, 7.f,
      5.f, 6.f, 7.f,
    };
    
    const math::mat3 mat_a = math::mat3_init(mat_data_a);
    
    const float mat_data_b[] {
      1.f, 2.f, 3.f,
      1.f, 2.f, 3.f,
      1.f, 2.f, 3.f,
    };
    
    const math::mat3 mat_b = math::mat3_init(mat_data_b);
    
    REQUIRE(math::mat3_is_near(mat_a, mat_b) == false);
    REQUIRE(math::mat3_is_not_near(mat_a, mat_b) == true);
  }
}
