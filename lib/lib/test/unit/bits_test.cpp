#include <catch/catch.hpp>
#include <lib/utilities.hpp>


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

  SECTION("Bit Pack/UnPack 32-32")
  {
    const uint32_t u32_a = 1234;
    const uint32_t u32_b = 5678;
    const uint64_t pack = lib::bits::pack3232(u32_a, u32_b);

    REQUIRE(lib::bits::lower32(pack) == u32_a);
    REQUIRE(lib::bits::upper32(pack) == u32_b);
  }
  
  SECTION("Bit Pack/Unpack 8-8-8-8")
  {
    const uint8_t a = 128;
    const uint8_t b = 255;
    const uint8_t c = 64;
    const uint8_t d = 123;
    
    const uint32_t pack = lib::bits::pack8888(a, b, c, d);
    
    REQUIRE(lib::bits::first8(pack) == a);
    REQUIRE(lib::bits::second8(pack) == b);
    REQUIRE(lib::bits::third8(pack) == c);
    REQUIRE(lib::bits::forth8(pack) == d);
  }
  
  SECTION("Bit Pack/Unpack 8-24")
  {
    const uint8_t a = 0xFF - 1;
    const uint32_t b = 0xFFFFFF - 1;
    
    const uint32_t pack = lib::bits::pack824(a, b);
    
    REQUIRE(lib::bits::first8(pack) == a);
    REQUIRE(lib::bits::upper24(pack) == b);
  }
}
