#include <catch/catch.hpp>
#include <lib/platform.hpp>
#include <stdio.h>

TEST_CASE("Config")
{
  SECTION("Has a platform")
  {
    uint32_t count = 0;

    if(lib::platform::is_mac_platform())
    {
      ++count;
      REQUIRE(lib::platform::is_nix_platform());
    }

    if(lib::platform::is_linux_platform())
    {
      ++count;
      REQUIRE(lib::platform::is_nix_platform());
    }

    if(lib::platform::is_windows_platform()) { ++count; }

    REQUIRE(count == 1);
  }

  SECTION("Has a compiler")
  {
    uint32_t count = 0;

    if(lib::platform::is_clang_compiler()) { ++count; }
    if(lib::platform::is_gcc_compiler())   { ++count; }
    if(lib::platform::is_msvs_compiler())  { ++count; }

    REQUIRE(count == 1);
  }
}
