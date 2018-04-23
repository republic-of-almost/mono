#include <catch/catch.hpp>
#include <roa_math/math.h>
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
    
    roa_mat4 expected;
    roa_mat4_import(&expected, mat_data);

    roa_mat4 mat;
    roa_mat4_id(&mat);

    REQUIRE(roa_mat4_is_near(&expected, &mat, ROA_EPSILON));
  }
  
  SECTION("Zero Mat")
  {
    const float mat_data[] {
      0.f, 0.f, 0.f, 0.f,
      0.f, 0.f, 0.f, 0.f,
      0.f, 0.f, 0.f, 0.f,
      0.f, 0.f, 0.f, 0.f,
    };
    
    roa_mat4 expected;
    roa_mat4_import(&expected, mat_data);

    roa_mat4 mat;
    roa_mat4_zero(&mat);

    REQUIRE(roa_mat4_is_near(&expected, &mat, ROA_EPSILON));
  }
  
  // --------------------------------------------------------- [ Initialize ] --
  
  SECTION("Init with Value")
  {
    const float mat_data[]{
      3.f, 3.f, 3.f, 3.f,
      3.f, 3.f, 3.f, 3.f,
      3.f, 3.f, 3.f, 3.f,
      3.f, 3.f, 3.f, 3.f,
    };

    roa_mat4 expected;
    roa_mat4_import(&expected, mat_data);

    roa_mat4 mat;
    roa_mat4_fill(&mat, 3.f);

    REQUIRE(roa_mat4_is_near(&expected, &mat, ROA_EPSILON));
  }
  
  
  // --------------------------------------------- [ Special Init With Data ] --
  
  /*
  SECTION("Scale Mat")
  {
    const float expected_mat_data[] {
      1.f, 0.f, 0.f, 0.f,
      0.f, 2.f, 0.f, 0.f,
      0.f, 0.f, 3.f, 0.f,
      0.f, 0.f, 0.f, 1.f,
    };
    
    const math::mat4 mat_expected = math::mat4_init(expected_mat_data);
    const math::mat4 mat_scale = math::mat4_scale(1.f, 2.f, 3.f);
    
    REQUIRE(math::mat4_is_near(mat_expected, mat_scale));
  }
	*/
  
	/*
  SECTION("Translate Mat")
  {
    const float expected_mat_data[] {
      1.f, 0.f, 0.f, 0.f,
      0.f, 1.f, 0.f, 0.f,
      0.f, 0.f, 1.f, 0.f,
      1.f, 2.f, 3.f, 1.f,
    };
    
    const math::mat4 mat_expected = math::mat4_init(expected_mat_data);
    const math::mat4 mat_translate = math::mat4_translate(1.f, 2.f, 3.f);
    
    REQUIRE(math::mat4_is_near(mat_expected, mat_translate));
  }
	*/

  // -------------------------------------------------------- [ Camera Mats ] --
  
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
  
	
  /*
  SECTION("Addition")
  {
    const float mat_data_1[] {
      1.f, 1.f, 1.f, 1.f,
      1.f, 1.f, 1.f, 1.f,
      1.f, 1.f, 1.f, 1.f,
      1.f, 1.f, 1.f, 1.f,
    };
    
    roa_mat4 mat_a;
    roa_mat4_import(&mat_a, mat_data_1);
    
    const float mat_data_2[] {
      2.f, 2.f, 2.f, 2.f,
      2.f, 2.f, 2.f, 2.f,
      2.f, 2.f, 2.f, 2.f,
      2.f, 2.f, 2.f, 2.f,
    };
    
    roa_mat4 mat_b;
    roa_mat4_import(&mat_b, mat_data_2);
    
    const float mat_expected_data[] {
      3.f, 3.f, 3.f, 3.f,
      3.f, 3.f, 3.f, 3.f,
      3.f, 3.f, 3.f, 3.f,
      3.f, 3.f, 3.f, 3.f,
    };
    
    roa_mat4 mat_expected;
    roa_mat4_import(&mat_expected, mat_expected_data);

    roa_mat4 mat_final;
    roa_mat4_add(&mat4_final, &mat_a, &mat_b);
    
    REQUIRE(roa_mat4_is_near(mat_expected, mat_final, ROA_EPSILON));
  }
  */
  
	/*
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
	*/

	/*
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
	*/
  
	/*
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
	*/
  
	/*
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
	*/
  
	/*
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
	*/
  
	/*
  SECTION("Transpose")
  {
    const float mat_data[] {
      1.f, 2.f, 3.f, 4.f,
      1.f, 2.f, 3.f, 4.f,
      1.f, 2.f, 3.f, 4.f,
      1.f, 2.f, 3.f, 4.f,
    };
    
    const math::mat4 mat_a = math::mat4_init(mat_data);
    const math::mat4 transpose = math::mat4_transpose(mat_a);
    
    const float mat_expected_data[] {
      1.f, 1.f, 1.f, 1.f,
      2.f, 2.f, 2.f, 2.f,
      3.f, 3.f, 3.f, 3.f,
      4.f, 4.f, 4.f, 4.f,
    };
    
    const math::mat4 mat_expected = math::mat4_init(mat_expected_data);
    
    REQUIRE(math::mat4_is_near(transpose, mat_expected));
  }
	*/
  
	
  SECTION("Inverse")
  {
    const float mat_data[] {
      1.f, 2.f, 3.f, 0.f,
      0.f, 5.f, 6.f, 0.f,
      0.f, 8.f, 9.f, 0.f,
      10.f, 11.f, 12.f, 1.f,
    };
    
    roa_mat4 mat_a;
    roa_mat4_import(&mat_a, mat_data);

    roa_mat4 inverse;
    roa_mat4_inverse(&inverse, &mat_a);
    
    const float mat_expected_data[] {
      1.f, -2.f, 1.f, 0.f,
      0.f, -3.f, 2.f, 0.f,
      0.f, 8.f / 3.f, -5.f / 3.f, 0.f,
      -10.f, 63.f / 3.f, -12.f, 1.f,
    };
    
    roa_mat4 expected;
    roa_mat4_import(&expected, mat_expected_data);

    REQUIRE(roa_mat4_is_near(&inverse, &expected, 0.00001f));
  }

  SECTION("Inverse ID")
  {
    roa_mat4 mat_a;
    roa_mat4_id(&mat_a);

    roa_mat4 inverse;
    roa_mat4_inverse(&inverse, &mat_a);

    roa_mat4 expected;
    roa_mat4_id(&expected);

    REQUIRE(roa_mat4_is_near(&inverse, &expected, 0.00001f));
  }
  

  SECTION("Determinant")
  {
    const float mat_data[] {
      1.f, 2.f, 3.f, 0.f,
      0.f, 5.f, 6.f, 0.f,
      0.f, 8.f, 9.f, 0.f,
      10.f, 11.f, 12.f, 1.f,
    };
    
    roa_mat4 mat_a;
    roa_mat4_import(&mat_a, mat_data);

    const float determinant = roa_mat4_determinant(&mat_a);
    
    REQUIRE(roa_float_is_near(determinant, -3.f, ROA_EPSILON));
  }
  
  // --------------------------------------------------------------- [ Data ] --
  
	/*
  SECTION("Get Sub")
  {
    const float mat_data_a[] {
      1.f, 2.f, 3.f, 4.f,
      4.f, 5.f, 6.f, 4.f,
      7.f, 8.f, 9.f, 4.f,
      1.f, 2.f, 3.f, 4.f,
    };
    
    const math::mat4 mat_a = math::mat4_init(mat_data_a);
    const math::mat3 sub_mat = math::mat4_get_sub_mat3(mat_a);
    
    for(uint32_t i = 0; i < 9; ++i)
    {
      REQUIRE(sub_mat.data[i] == i + 1);
    }
  }
	*/
  
	/*
  SECTION("Get")
  {
    const float expected_mat_data[] {
      0.f, 1.f, 2.f, 3.f,
      4.f, 5.f, 6.f, 7.f,
      8.f, 9.f, 10.f, 11.f,
      12.f, 13.f, 14.f, 15.f,
    };
    
    const math::mat4 mat_a = math::mat4_init(expected_mat_data);
    
    REQUIRE(math::mat4_get(mat_a, 0, 0) == 0.f);
    REQUIRE(math::mat4_get(mat_a, 1, 0) == 4.f);
    REQUIRE(math::mat4_get(mat_a, 3, 3) == 15.f);
  }
	*/
  
	/*
  SECTION("Data")
  {
    const float expected_mat_data[] {
      0.f, 1.f, 2.f, 3.f,
      4.f, 5.f, 6.f, 7.f,
      8.f, 9.f, 10.f, 11.f,
      12.f, 13.f, 14.f, 15.f,
    };
    
    const math::mat4 mat_a = math::mat4_init(expected_mat_data);
    
    const float *mat_data = math::mat4_data(mat_a);
    
    for(uint32_t i = 0; i < 16; ++i)
    {
      REQUIRE(expected_mat_data[i] == mat_data[i]);
    }
  }
	*/
  
	/*
  SECTION("To Array")
  {
    const float expected_mat_data[] {
      0.f, 1.f, 2.f, 3.f,
      4.f, 5.f, 6.f, 7.f,
      8.f, 9.f, 10.f, 11.f,
      12.f, 13.f, 14.f, 15.f,
    };
    
    const math::mat4 mat_a = math::mat4_init(expected_mat_data);
    
    float export_mat[16]{};
    math::mat4_to_array(mat_a, export_mat);
    
    for(uint32_t i = 0; i < 16; ++i)
    {
      REQUIRE(expected_mat_data[i] == export_mat[i]);
    }
  }
	*/
  
  // ----------------------------------------------------------- [ Equality ] --
  
	/*
  SECTION("Equal")
  {
    const float mat_data_a[] {
      1.f, 2.f, 3.f, 4.f,
      1.f, 2.f, 3.f, 4.f,
      1.f, 2.f, 3.f, 4.f,
      1.f, 2.f, 3.f, 4.f,
    };
    
    const math::mat4 mat_a = math::mat4_init(mat_data_a);
    
    const float mat_data_b[] {
      1.f, 2.f, 3.f, 4.f,
      1.f, 2.f, 3.f, 4.f,
      1.f, 2.f, 3.f, 4.f,
      1.f, 2.f, 3.f, 4.f,
    };
    
    const math::mat4 mat_b = math::mat4_init(mat_data_b);
    
    REQUIRE(math::mat4_is_near(mat_a, mat_b) == true);
    REQUIRE(math::mat4_is_not_near(mat_a, mat_b) == false);
  }
	*/
  
	/*
  SECTION("Not Equal")
  {
    const float mat_data_a[] {
      5.f, 6.f, 7.f, 8.f,
      5.f, 6.f, 7.f, 8.f,
      5.f, 6.f, 7.f, 8.f,
      5.f, 6.f, 7.f, 8.f,
    };
    
    const math::mat4 mat_a = math::mat4_init(mat_data_a);
    
    const float mat_data_b[] {
      1.f, 2.f, 3.f, 4.f,
      1.f, 2.f, 3.f, 4.f,
      1.f, 2.f, 3.f, 4.f,
      1.f, 2.f, 3.f, 4.f,
    };
    
    const math::mat4 mat_b = math::mat4_init(mat_data_b);
    
    REQUIRE(math::mat4_is_near(mat_a, mat_b) == false);
    REQUIRE(math::mat4_is_not_near(mat_a, mat_b) == true);
  }
	*/
}
