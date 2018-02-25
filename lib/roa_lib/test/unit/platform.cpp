#include <roa_lib/fundamental.h>
#include <roa_lib/platform.h>
#include <catch/catch.hpp>


/* --------------------------------------------------------- [ Test Case ] -- */


TEST_CASE("Platform")
{
  SECTION("Compiler")
  {
    #ifdef _MSC_VER
    REQUIRE(!!ROA_IS_ENABLED(ROA_COMPILER_IS_MSVS) == true);
    REQUIRE(!!ROA_IS_ENABLED(ROA_COMPILER_IS_CLANG) == false);
    REQUIRE(!!ROA_IS_ENABLED(ROA_COMPILER_IS_GCC) == false);
    #endif

    #ifdef __clang__
    REQUIRE(ROA_IS_ENABLED(ROA_COMPILER_IS_MSVS) == false);
    REQUIRE(ROA_IS_ENABLED(ROA_COMPILER_IS_CLANG) == true);
    REQUIRE(ROA_IS_ENABLED(ROA_COMPILER_IS_GCC) == false);
    #endif

    #ifdef GNU
    REQUIRE(!!ROA_IS_ENABLED(ROA_COMPILER_IS_MSVS) == false);
    REQUIRE(!!ROA_IS_ENABLED(ROA_COMPILER_IS_CLANG) == false);
    REQUIRE(!!ROA_IS_ENABLED(ROA_COMPILER_IS_GCC) == true);
    #endif
  }

  SECTION("Platform")
  {
    #ifdef _WIN32
    REQUIRE(ROA_IS_ENABLED(ROA_OS_IS_WINDOWS) == true);
    REQUIRE(ROA_IS_ENABLED(ROA_OS_IS_APPLE) == false);
    REQUIRE(ROA_IS_ENABLED(ROA_OS_IS_NIX) == false);
    REQUIRE(ROA_IS_ENABLED(ROA_OS_IS_LINUX) == false);
    #endif

    #ifdef _APPLE_
    REQUIRE(ROA_IS_ENABLED(ROA_OS_IS_WINDOWS) == false);
    REQUIRE(ROA_IS_ENABLED(ROA_OS_IS_APPLE) == true);
    REQUIRE(ROA_IS_ENABLED(ROA_OS_IS_NIX) == true);
    REQUIRE(ROA_IS_ENABLED(ROA_OS_IS_LINUX) == false);
    #endif

    #ifdef __linux__
    REQUIRE(ROA_IS_ENABLED(ROA_OS_IS_WINDOWS) == false);
    REQUIRE(ROA_IS_ENABLED(ROA_OS_IS_APPLE) == false);
    REQUIRE(ROA_IS_ENABLED(ROA_OS_IS_NIX) == true);
    REQUIRE(ROA_IS_ENABLED(ROA_OS_IS_LINUX) == true);
    #endif
  }
}
