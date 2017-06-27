#include <catch/catch.hpp>
#include <lib/bits.hpp>


TEST_CASE("Bits")
{
  SECTION("Bit n")
  {
    REQUIRE(1 == BIT(0));
    REQUIRE(2 == BIT(1));
    REQUIRE(4 == BIT(2));
    REQUIRE(8 == BIT(3));
    REQUIRE(16 == BIT(4));
  }

  SECTION("Bit Pack/UnPack")
  {
    const uint32_t u32_a = 1234;
    const uint32_t u32_b = 5678;
    const uint64_t pack = lib::bits::pack3232(u32_a, u32_b);

    REQUIRE(lib::bits::lower32(pack) == u32_a);
    REQUIRE(lib::bits::upper32(pack) == u32_b);
  }
}
