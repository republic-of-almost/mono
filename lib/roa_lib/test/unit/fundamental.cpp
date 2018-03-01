#include <roa_lib/fundamental.h>
#include <catch/catch.hpp>


/* --------------------------------------------------------- [ Test Case ] -- */


TEST_CASE("Fundamental")
{
  SECTION("Fixed Types")
  {
    /* catch imports fixed types so can't test here easily */
    /* unless I typedef them to roa_int8_t but that would be silly */
    REQUIRE(true);
  }

  SECTION("Bool")
  {
    ROA_BOOL roa_true = ROA_TRUE;
    ROA_BOOL roa_false = ROA_FALSE;

    REQUIRE(!!roa_true == true);
    REQUIRE(!!roa_false == false);
  }

  SECTION("Null")
  {
    REQUIRE(ROA_NULL == 0);
    REQUIRE(ROA_NULL == NULL);
  }

  SECTION("Enabled")
  {
#define TEST_SHOULD_FAIL 0
#define TEST_SHOULD_PASS 1

    if (!ROA_IS_ENABLED(TEST_SHOULD_FAIL))
    {
      REQUIRE(true);
    }
    else
    {
      REQUIRE(false);
    }

    if (!!ROA_IS_ENABLED(TEST_SHOULD_PASS))
    {
      REQUIRE(true);
    }
    else
    {
      REQUIRE(false);
    }
  }

  SECTION("Array")
  {
    int arr[] = { 1,2,3,4,5,6 };
    REQUIRE(ROA_ARR_COUNT(arr) == 6);
    REQUIRE(ROA_ARR_DATA(arr) == &arr[0]);
  }

  SECTION("Memcpy")
  {
    {
      int src[] = { 1,2,3 };
      int dst[ROA_ARR_COUNT(src)]{};

      ROA_MEM_CPY(dst, src);

      unsigned count = ROA_ARR_COUNT(src);

      for (unsigned i = 0; i < count; ++i)
      {
        REQUIRE(src[i] == dst[i]);
      }
    }

    {
      char src[] = "hello";
      char dst[ROA_ARR_COUNT(src)]{};


      ROA_MEM_CPY(dst, src);

      unsigned count = ROA_ARR_COUNT(src);

      for (unsigned i = 0; i < count; ++i)
      {
        REQUIRE(src[i] == dst[i]);
      }
    }
  }

  SECTION("Memset")
  {
    int src[] = {1,2,3};

    ROA_MEM_ZERO(src);

    unsigned count = ROA_ARR_COUNT(src);

    for (unsigned i = 0; i < count; ++i)
    {
      REQUIRE(src[i] == 0);
    }
  }
}
