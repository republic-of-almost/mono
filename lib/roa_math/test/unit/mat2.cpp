#include <catch/catch.hpp>
#include <roa_math/math.h>


TEST_CASE("Matrix 2x2")
{

	// ---------------------------------------------------------- [ Constants ] --
	
	SECTION("ID Mat")
	{
		const float mat_data[] {
			1.f, 0.f,
			0.f, 1.f,
		};

		roa_mat2 expected;
		roa_mat2_import(&expected, mat_data);

		roa_mat2 mat;
		roa_mat2_id(&mat);

		REQUIRE(roa_mat2_is_near(&expected, &mat, ROA_EPSILON));
	}


	SECTION("Zero Mat")
	{
		const float mat_data[] {
			0.f, 0.f,
			0.f, 0.f,
		};

		roa_mat2 expected;
		roa_mat2_import(&expected, mat_data);

		roa_mat2 mat;
		roa_mat2_zero(&mat);

		REQUIRE(roa_mat2_is_near(&expected, &mat, ROA_EPSILON));
	}


	// --------------------------------------------------------- [ Initialize ] -- 


	SECTION("Fill")
	{
		const float mat_data[] {
			3.f, 3.f,
			3.f, 3.f,
		};

		roa_mat2 expected;
		roa_mat2_import(&expected, mat_data);

		roa_mat2 mat;
		roa_mat2_fill(&mat, 3.f);

		REQUIRE(roa_mat2_is_near(&expected, &mat, ROA_EPSILON));
	}

	
	// --------------------------------------------------------- [ Arithmetic ] -- 


	SECTION("Multiply")
	{
	}


	SECTION("Multiply with Vector")
	{
	}

}
