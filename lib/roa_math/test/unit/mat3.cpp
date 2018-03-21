#include <catch/catch.hpp>
#include <roa_math/math.h>


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
    
    roa_mat3 expected;
		roa_mat3_import(&expected, mat_data);

		roa_mat3 mat;
		roa_mat3_id(&mat);
    
    REQUIRE(roa_mat3_is_near(&expected, &mat, ROA_EPSILON));
  }
	
  
	
  SECTION("Zero Mat")
  {
    const float mat_data[] {
      0.f, 0.f, 0.f,
      0.f, 0.f, 0.f,
      0.f, 0.f, 0.f,
    };

		roa_mat3 expected;
		roa_mat3_import(&expected, mat_data);

		roa_mat3 mat;
		roa_mat3_zero(&mat);

		REQUIRE(roa_mat3_is_near(&expected, &mat, ROA_EPSILON));
  }
	
  
  // --------------------------------------------------------- [ Initialize ] --
  
  
  SECTION("Init with Value")
  {
    const float mat_data[] {
      3.f, 3.f, 3.f,
      3.f, 3.f, 3.f,
      3.f, 3.f, 3.f,
    };

		roa_mat3 expected;
		roa_mat3_import(&expected, mat_data);

		roa_mat3 mat;
		roa_mat3_fill(&mat, 3.f);
    
    REQUIRE(roa_mat3_is_near(&expected, &mat, ROA_EPSILON));
  }
 

  // --------------------------------------------- [ Special Init With Data ] --
  
	/*
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
	*/
  
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
   
		roa_mat3 mat_a;
		roa_mat3_import(&mat_a, mat_a_data);
    
    const float mat_b_data[] {
      2.f, 2.f, 2.f,
      2.f, 2.f, 2.f,
      2.f, 2.f, 2.f,
    };
    
		roa_mat3 mat_b;
		roa_mat3_import(&mat_b, mat_b_data);

    const float mat_c_data[] {
      3.f, 3.f, 3.f,
      3.f, 3.f, 3.f,
      3.f, 3.f, 3.f,
    };
    
    roa_mat3 mat_c;
		roa_mat3_import(&mat_c, mat_c_data);

		roa_mat3 result;
		roa_mat3_add(&result, &mat_a, &mat_b);
    
    REQUIRE(roa_mat3_is_near(&mat_c, &result, ROA_EPSILON));
  }
	

	
  SECTION("Subtraction")
  {
    const float mat_a_data[] {
      1.f, 1.f, 1.f,
      1.f, 1.f, 1.f,
      1.f, 1.f, 1.f,
    };
   
		roa_mat3 mat_a;
		roa_mat3_import(&mat_a, mat_a_data);
    
    const float mat_b_data[] {
      2.f, 2.f, 2.f,
      2.f, 2.f, 2.f,
      2.f, 2.f, 2.f,
    };
   
		roa_mat3 mat_b;
		roa_mat3_import(&mat_b, mat_b_data);
    
    const float mat_c_data[] {
      -1.f, -1.f, -1.f,
      -1.f, -1.f, -1.f,
      -1.f, -1.f, -1.f,
    };
   
		roa_mat3 mat_c;
		roa_mat3_import(&mat_c, mat_c_data);

		roa_mat3 result;
		roa_mat3_subtract(&result, &mat_a, &mat_b);
    
    REQUIRE(roa_mat3_is_near(&mat_c, &result, ROA_EPSILON));
  }
	

  SECTION("Multiplication (mat * mat)")
  {
    const float mat_a_data[] {
      1.f, 1.f, 1.f,
      1.f, 1.f, 1.f,
      1.f, 1.f, 1.f,
    };
    
		roa_mat3 mat_a;
		roa_mat3_import(&mat_a, mat_a_data);
    
    const float mat_b_data[] {
      2.f, 2.f, 2.f,
      2.f, 2.f, 2.f,
      2.f, 2.f, 2.f,
    };
    
		roa_mat3 mat_b;
		roa_mat3_import(&mat_b, mat_b_data);
    
    const float mat_c_data[] {
      6.f, 6.f, 6.f,
      6.f, 6.f, 6.f,
      6.f, 6.f, 6.f,
    };
    
		roa_mat3 mat_c;
		roa_mat3_import(&mat_c, mat_c_data);

		roa_mat3 result;
		roa_mat3_multiply(&result, &mat_a, &mat_b);

		REQUIRE(roa_mat3_is_near(&mat_c, &result, ROA_EPSILON));
  }
  
	
  SECTION("Multiplication (mat * vec)")
  {
    const float mat_a_data[] {
      1.f, 1.f, 1.f,
      1.f, 1.f, 1.f,
      1.f, 1.f, 1.f,
    };

		roa_mat3 mat_a;
		roa_mat3_import(&mat_a, mat_a_data);

		roa_float3 vec = roa_float3_set_with_values(1.f, 2.f, 3.f);
		roa_float3 vec_result = roa_mat3_multiply_with_float3(vec, &mat_a);

		roa_float3 expected = roa_float3_set_with_values(6.f, 6.f, 6.f);

		REQUIRE(roa_float3_is_near(expected, vec_result, ROA_EPSILON));
  }
	
  
	/*
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
	*/
  
  SECTION("Determinant")
  {
  }
  
  // --------------------------------------------------------------- [ Data ] --
  
	/*
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
	*/
  
	/*
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
	*/
  
  // ----------------------------------------------------------- [ Equality ] --
  
	/*
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
	*/
  
	/*
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
	*/
}
